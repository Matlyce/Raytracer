/*
** EPITECH PROJECT, 2024
** Raytracer
** File description:
** Parser
*/

#ifndef PARSER_HPP_
    #define PARSER_HPP_

    #include <libconfig.h++>
    #include <libconfig.h>

    #include "Core.hpp"

namespace RayTracer {
    struct config_camere_s {
        int resolution[2];
        double position[3];
        double rotation[3];
        double fov;
    };
    class Parser {
        public:
            class ParserException : public std::exception {
                public:
                    ParserException(const std::string &message) : _message(message) {};
                    const char *what() const noexcept override { return _message.c_str(); }

                private:
                    std::string _message;
            };
            Parser(Core&, const std::string&);
            ~Parser();

            void parseConfig();
            void printConfig();

            config_camere_s camera_settings;
            Core &refCore;

        protected:
        private:
            std::string _path;
            libconfig::Config _cfg;

            // CAMERA SETTINGS
            libconfig::Setting *_cameraSection;

            void parseCameraSettings();

            // PRIMITIVES
            libconfig::Setting *_primitivesSection;

            void parsePrimitives();
            void parseSpheres();
            void parsePlanes();
            RayTracer::Plane::AXIS getPlaneAxis(const std::string&);
            Math::Point3D getPlaneOrigin(double, RayTracer::Plane::AXIS);
    };
}

#endif /* !PARSER_HPP_ */
