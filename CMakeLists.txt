cmake_minimum_required(VERSION 3.10)
project(SIC_XE)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(SIC_XE
        file_parse_exception.h
        file_parser.cc
        file_parser.h
        opcode_error_exception.h
        opcodetab.cc
        opcodetab.h
        sicxe_asm.cpp
        sicxe_asm.h
        symtab.cc
        symtab.h
        symtab_exception.h Driver.cpp)
