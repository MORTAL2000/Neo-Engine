#pragma once

#include "Window/Window.hpp"

namespace neo {

    class NeoEngine {
    
        public:
            static void init(const std::string &, const int, const int);
            static void run();
            static void shutDown();

            static std::string RESOURCE_DIR;    /* Resource directory */
            static std::string APP_NAME;        /* Name of application */
            static int FPS;                     /* Frames per second */
            static double timeStep;             /* Delta time */

        private:
            static void updateFPS();
            static double lastFPSTime;      /* Time at which last FPS was calculated */
            static int nFrames;             /* Number of frames in current second */
            static double lastFrameTime;    /* Time at which last frame was rendered */
            static double runTime;          /* Global timer */


    };
}