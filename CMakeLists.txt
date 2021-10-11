# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sensor_data_adv)

target_sources(app PRIVATE src/main.c
                            src/services/custom_service.h
                            src/utils/rssi.c)

target_include_directories(app PRIVATE src/services
                                        src/utils)
