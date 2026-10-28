#pragma once

namespace common    {
    class application   {
        public:
            application() = default;
            virtual ~application() = default;

            void run() {
                setup();
                while (running) {
                    loop();
                }
                shutdown();
            }
            void quit() {
                running = false;
            }
        protected:
            virtual void setup() = 0;
            virtual void loop() = 0;
            virtual void shutdown() = 0;
        private:
            bool running = true;
    };
}

using Application = common::application;
