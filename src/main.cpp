/*
** EPITECH PROJECT, 2024
** RAYTRACER
** File description:
** main
*/

#include "../includes/Core.hpp"
#include "../plugins/primitives/sphere/includes/Sphere.hpp"
#include "../plugins/primitives/plane/includes/Plane.hpp"
#include "../includes/Parser.hpp"
#include "../includes/Light.hpp"

#include "../includes/Workers.hpp"

#include "../includes/SFMLDisplay.hpp"

static bool hasOption(int ac, char **av, const std::string &option)
{
    for (int i = 0; i < ac; i++) {
        if (std::string(av[i]) == option)
            return true;
    }
    return false;
}

static void *getOptionValue(int ac, char **av, const std::string &option, const std::string &shortOption = "")
{
    for (int i = 0; i < ac; i++) {
        if (std::string(av[i]) == option)
            return av[i + 1];
        if (shortOption != "" && std::string(av[i]) == shortOption)
            return av[i + 1];
    }
    return nullptr;
}

int main(int ac, char **av)
{
    std::unique_ptr<Graphics::AGraphicals> display;

    if (hasOption(ac, av, "-h") || hasOption(ac, av, "--help")) {
        std::cerr << "Usage: ./raytracer [options] > [image_path]" << std::endl;
        std::cerr << "\nOptions:" << std::endl;
        std::cerr << "\t-h, --help:\t Display this help message" << std::endl;
        std::cerr << "\t-t, --threads:\t Set the numbers of threads used for rendering" << std::endl;
        std::cerr << "\t-g, --gui:\t Enable the GUI" << std::endl;
        return 0;
    }

    bool threadsOption = hasOption(ac, av, "-t") || hasOption(ac, av, "--threads");
    int threadsCount = 1;
    if (threadsOption) {
        threadsCount = std::stoi(static_cast<const char *>(getOptionValue(ac, av, "--threads", "-t")));
    }

    bool guiOption = hasOption(ac, av, "-g") || hasOption(ac, av, "--gui");

    // RayTracer::Core core(400, 400);
    // Workers workers(threadsCount, 400, 400);
    std::shared_ptr<RayTracer::Core> core = std::make_shared<RayTracer::Core>(400, 400);
    std::shared_ptr<Workers> workers = std::make_shared<Workers>(threadsCount, 400, 400);
    core->loadLibrairies();

    if (guiOption) {
        display = std::make_unique<Graphics::SFML::SFMLDisplay>();
        display->setup(core, workers);
        display->createWindow(1280, 720, "RayTracer");
    }

    // RayTracer::Parser parser(core, "./configs/subject_config");
    // parser.printConfig();

    double sphereRadius = 2.0;

    auto material_ground = std::make_shared<RayTracer::Material::Lambertian>(RayTracer::Color(255, 255, 0));
    auto material_center = std::make_shared<RayTracer::Material::Lambertian>(RayTracer::Color(0, 255, 255));
    auto material_left   = std::make_shared<RayTracer::Material::Metal>(RayTracer::Color(255, 0, 0), 0.0);
    auto material_right  = std::make_shared<RayTracer::Material::LightDiffuse>(RayTracer::Color(255, 255, 255));

    Math::Point3D sphere_left_pos(2.5, 8, 20);
    RayTracer::Sphere sphere_left(sphere_left_pos, sphereRadius, material_left);

    Math::Point3D sphere_center_pos(7.5, 8, 20);
    RayTracer::Sphere sphere_center(sphere_center_pos, sphereRadius, material_center);

    Math::Point3D sphere_ground_pos(7.5, 108, 0);
    RayTracer::Sphere sphere_ground(sphere_ground_pos, 100, material_ground);

    Math::Point3D plane_position(0, 10, 0);
    RayTracer::Plane plane(plane_position, RayTracer::Plane::AXIS::Y, material_ground);

    Math::Point3D sphere_light1_pos(12.5, 8, 10);
    Math::Point3D sphere_light2_pos(12.5, 8, 20);
    Math::Point3D sphere_light3_pos(7.5, 0, 20);

    RayTracer::Sphere sphere_light1(sphere_light1_pos, sphereRadius, material_right);
    RayTracer::Sphere sphere_light2(sphere_light2_pos, sphereRadius, material_right);
    RayTracer::Sphere sphere_light3(sphere_light3_pos, sphereRadius, material_right);

    core->addShape(sphere_light1);
    core->addShape(sphere_light2);
    core->addShape(sphere_light3);
    core->addShape(plane);

    // core->addShape(sphere_ground);
    core->addShape(sphere_center);
    core->addShape(sphere_left);

    std::cerr << "Rendering settings: " << std::endl;
    std::cerr << " - Width: " << core->_camera._image_width << std::endl;
    std::cerr << " - Height: " << core->_camera._image_height << std::endl;
    std::cerr << " - Quality (samples): " << core->_camera._samples << std::endl;
    std::cerr << " - Max depth: " << core->_maxDepth << std::endl;
    std::cerr << " - Threads: " << workers->getThreadsCount() << std::endl;

    if (guiOption) {
        std::cerr << "Rendering with GUI" << std::endl;
        while (display->isWindowOpen()) {
            display->renderAll();
        }
    } else {
        workers->initialize(*core);
        // workers->render(*core);
        workers->setRendering(true);
        workers->beginRender();
        while (workers->isRendering())
            workers->renderUpdate(*core);
    }
}
