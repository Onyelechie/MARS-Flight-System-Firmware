#[[
MIT License

Copyright (c) 2023 limitless Aeronautics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]


idf_component_register(SRCS "base-firmware.cpp"
                            "../components/Comms/_broadcast.cpp"
                            "../components/HALX/Servo/mg90s_servo.cpp"
                            "../components/HALX/Display/ssd1306.cpp"
                            "../components/HALX/Fan_cooling/fan_relay.cpp"
                            "../components/HALX/Barometer/_barometerEntry.cpp"
                            "../components/PTAM/_ptam.cpp"
                            "../components/system/validateSensors.cpp"
                            "../components/system/_state.cpp"
                            "../components/system/sys_controller.cpp"
                            "../components/Logging/logger.cpp"

                    INCLUDE_DIRS ".")
