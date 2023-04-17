#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"

#include <float.h>
#include <iostream>

#include <fstream>
#include <chrono>
#include <string>

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string.h>

unsigned int counter;

// Defining color of a ray
color ray_color(const ray &a, hittable *world, int depth, const int maxdepth)
{
    hit_record rec;
    if (world->hit(a, 0.001, FLT_MAX, rec))
    {
        // If a ray reached any object
        ray scattered;
        vec3 attenuation;
        // Do this recursion "depth" times or until the ray stops hitting anything
        if (depth < maxdepth && rec.mat_ptr->scatter(a, rec, attenuation, scattered))
        {
            // This results in the shadow effect
            return attenuation * ray_color(scattered, world, depth + 1, maxdepth);
        }
        else
        {
            // If it hit more than "depth" times, return black
            // this means that a ray between two mirror object would also dissapear
            return vec3(0, 0, 0);
        }
    }
    else
    {
        // If hasn't reached anything, return the sky color
        vec3 unit_direction = unit_vector(a.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

// Defining the rendered scene
hittable_list *random_scene()
{
    int n = 500;
    hittable **list = new hittable *[n + 1];
    // the surface is just a big sphere
    list[0] = new sphere(vec3(0, -1000, 0), vec3(0, -1000, 0), 0.0, 1.0, 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_db(0, 1);
            point3 center(a + 0.9 * random_db(0, 1), 0.2, b + 0.9 * random_db(0, 1));

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.6)
                {
                    // diffuse
                    // center2 is a destination in wich the sphere travels (between some time duration)
                    // this results in a motion blur effect
                    auto center2 = center + vec3(0, random_db(0, 0.25), 0);
                    list[i++] = new sphere(center, center2, 0.0, 5.0, 0.2,
                                           new lambertian(vec3(random_db(0.0, 1.0) * random_db(0.0, 1.0),
                                                               random_db(0.0, 1.0) * random_db(0.0, 1.0),
                                                               random_db(0.0, 1.0) * random_db(0.0, 1.0))));
                }
                else if (choose_mat < 0.9)
                {
                    // metal
                    list[i++] = new sphere(center, center, 0.0, 1.0, 0.2,
                                           new metal(vec3(0.5 * (1 + random_db(0.0, 1.0)),
                                                          0.5 * (1 + random_db(0.0, 1.0)),
                                                          0.5 * (1 + random_db(0.0, 1.0))),
                                                     0.5 * random_db(0.0, 1.0)));
                }
                else
                {
                    // glass
                    list[i++] = new sphere(center, center, 0.0, 1.0, 0.2, new glass(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), vec3(0, 1, 0), 0.0, 1.0, 1.0, new glass(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), vec3(-4, 1, 0), 0.0, 1.0, 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), vec3(4, 1, 0), 0.0, 1.0, 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hittable_list(list, i);
}

// Job of each thread
struct BlockJob
{
    int rowStart;
    int rowEnd;
    int colSize;
    int spp;
    // index of each pixel
    std::vector<int> indices;
    // color of each pixel
    std::vector<vec3> colors;
};

// Definig colors of pixels assigned for each thread
void calculateColor(BlockJob job, std::vector<BlockJob> &imageBlocks, int ny, camera cam, hittable *world, std::mutex &mutex, std::condition_variable &cv, std::atomic<int> &completedThreads, int depth)
{
    for (int j = job.rowStart; j < job.rowEnd; j++)
    {
        for (int i = 0; i < job.colSize; i++)
        {
            vec3 col(0, 0, 0);
            // this loop is responsible for blending colors of adjacent pixels
            // the final color of a pixel is an average of these pixels
            for (int k = 0; k < job.spp; k++)
            {
                float u = float(i + random_db(0.0, 1.0)) / float(job.colSize);
                float v = float(j + random_db(0.0, 1.0)) / float(ny);
                ray r = cam.get_ray(u, v);
                col += ray_color(r, world, 0, depth);
            }
            col /= float(job.spp);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            const unsigned int index = j * job.colSize + i;
            job.indices.push_back(index);
            job.colors.push_back(col);
        }
        counter++;
        std::cerr << "\rScanlines remaining: " << ny - counter;
    }
    {
        // Only one thread simultaneously
        std::lock_guard<std::mutex> lock(mutex);
        imageBlocks.push_back(job);
        completedThreads++;
        cv.notify_one();
    }
}

int main()
{
    // Image
    float ratio = 4.0 / 3.0;
    int img_w = 1440;
    int img_h = static_cast<int>(img_w / ratio);
    int smpls_pr_px = 100;
    int depth = 50;
    int pixelCount = img_h * img_w;

    // World
    hittable *world = random_scene();

    // Camera
    point3 look_from(13, 2, 3);
    point3 look_at(0, 0, 0);
    vec3 vup(0, -1, 0);
    auto dist_to_focus = 10.0f;
    auto aperture = 0.1f;

    camera cam(look_from, look_at, vup, 20, ratio, aperture, dist_to_focus, 0.0, 5.0);
    
    // Image
    vec3 *image = new vec3[pixelCount];
    memset(&image[0], 0, pixelCount * sizeof(vec3));

    // Render
    auto fulltime = std::chrono::high_resolution_clock::now();
    // Define threads number
    const int nThreads = std::thread::hardware_concurrency();
    // The final render is divided into sections which are assigned to each thread
    // (Each thread renders only a part of the image)
    int rowsPerThread = img_h / nThreads;
    int leftOver = img_h % nThreads;

    std::mutex mutex;
    std::condition_variable cvResults;
    std::vector<BlockJob> imageBlocks;
    std::atomic<int> completedThreads = {0};
    std::vector<std::thread> threads;
    std::cout << nThreads;

    // Starting  multithreading
    for (int i = 0; i < nThreads; i++)
    {
        BlockJob job;
        job.rowStart = i * rowsPerThread;
        job.rowEnd = job.rowStart + rowsPerThread;
        if (i == nThreads - 1)
        {
            job.rowEnd = job.rowStart + rowsPerThread + leftOver;
        }
        job.colSize = img_w;
        job.spp = smpls_pr_px;

        std::thread t([job, &imageBlocks, img_h, &cam, &world, &mutex, &cvResults, &completedThreads, depth]()
                      { calculateColor(job, imageBlocks, img_h, cam, world, mutex, cvResults, completedThreads, depth); });
        threads.push_back(std::move(t));
    }

    // Wait until all the threads have finished thier job
    {
        std::unique_lock<std::mutex> lock(mutex);
        cvResults.wait(lock, [&completedThreads, &nThreads]
                       { return completedThreads = nThreads; });
    }

    // Join the threads
    for (std::thread &t : threads)
    {
        t.join();
    }

    // Move the pixels colors to one array
    for (BlockJob job : imageBlocks)
    {
        int index = job.rowStart;
        int colorIndex = 0;
        for (vec3 &col : job.colors)
        {
            int colIndex = job.indices[colorIndex];
            image[colIndex] = col;
            ++colorIndex;
        }
    }

    // Full time duration
    auto timeSpan = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - fulltime);
    int frameTimeMs = static_cast<int>(timeSpan.count());
    std::cout << std::endl
              << " - time " << frameTimeMs << " sec \n";

    // Render file name
    std::string filename = "Final_img";

    // Opening the file
    std::ofstream fileHandler;
    fileHandler.open(filename, std::ios::out | std::ios::binary | std::fstream::trunc);
    if (!fileHandler.is_open())
    {
        std::cout << "Couldn't open the file\n";
        return false;
    }
    // Setting the ppm file (P3 for a PPM file)
    // Setting the max color value (255)
    fileHandler << "P3\n"
                << img_w << ' ' << img_h << "\n255\n";

    // Writing to file
    for (unsigned int i = 0; i < pixelCount; i++)
    {
        fileHandler << static_cast<int>(255.99f * image[i].crd[2]) << ' '
                    << static_cast<int>(255.99f * image[i].crd[1]) << ' '
                    << static_cast<int>(255.99f * image[i].crd[0]) << '\n';
    }

    std::cout << "File Saved" << std::endl;
    fileHandler.close();
    
    delete[] image;
    delete[] world;
    return 0;
}
