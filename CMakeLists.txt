# cmake_minimum_required (VERSION 3.10)
# project(Gestion_Employes VERSION 1.0)

# # standard C++
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Gestion de Google Test
# include(FetchContent)
# FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
# )

# # Empecher gtest de modifier les options de compilation
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)


# # Bibliotheques externes (sqlite3 et sodium)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(SQLITE3 REQUIRED sqlite3)
# pkg_check_modules(SODIUM REQUIRED libsodium)


# #liste des fichiers sources
# set(SOURCES
#     src/main.cpp 
#     src/Administrateur.cpp
#     src/Employe.cpp 
#     src/Data_base.cpp 
#     src/Menu.cpp 
#     src/Connexion.cpp 
#     src/Admin_Data_Base.cpp 
#     src/qrcode.c
# )

# # Creation de l'executable
# add_executable(main ${SOURCES})


# #Listes des bib et des Headers
# target_link_libraries(main PRIVATE ${SQLITE3_LIBRARIES} ${SODIUM_LIBRARIES})
# target_include_directories(main PRIVATE ${SQLITE3_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS})
# target_include_directories(main PRIVATE include)


# #tests (executables de tests unitaires)
# add_executable(run_tests 
#                 tests/unit_tests.cpp 
#                 src/Data_base.cpp
#                 src/Employe.cpp
#                 # src/main.cpp 
#                 src/Administrateur.cpp
#                 # src/Employe.cpp 
#                 # src/Data_base.cpp 
#                 src/Menu.cpp 
#                 src/Connexion.cpp 
#                 src/Admin_Data_Base.cpp 
#                 src/qrcode.c)

# target_include_directories(run_tests PRIVATE include)
# target_include_directories(run_tests PRIVATE SQLite3::SQLite3 ${SQLITE3_INCLUDE_DIRS})
# target_link_libraries(run_tests
#                          PRIVATE 
#                          Gtest::gtest_main
#                          ${SQLITE3_LIBRARIES} 
#                          ${SODIUM_LIBRARIES}
#                         #  Threads::Threads
#                          )


# #lancer les tests
# add_test(NAME BaseTests COMMAND run_tests)

# #script pour tests manuela
# add_test(NAME MaFonction
#          COMMAND ${CMAKE_COMMAND} -DTEST_NAME=MaFonction -DTEST_EXE=$<TARGET_FILE:run_tests> -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.cmake")
#         #  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# #test GoogleTest
# include(GoogleTest)
# gtest_discover_tests(run_tests)



cmake_minimum_required (VERSION 3.11) # FetchContent nécessite au moins 3.11
cmake_policy(SET CMP0135 NEW)
project(Gestion_Employes VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Gestion de Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # 
)
# Empêche gtest de modifier les paramètres de compilation globaux
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- 2. Bibliothèques externes ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
pkg_check_modules(SODIUM REQUIRED libsodium)

#config manuels MySQL Connectors
find_library(MYSQL_LIB NAMES mysqlcppconn REQUIRED)
find_path(MYSQL_INCLUDE_DIR NAMES mysql_driver.h REQUIRED)

# --- 3. Exécutable Principal ---
set(SOURCES
    src/main.cpp 
    src/Administrateur.cpp
    src/Employe.cpp 
    src/Data_base.cpp 
    src/Menu.cpp 
    src/Connexion.cpp 
    src/Admin_Data_Base.cpp 
    src/qrcode.c
    src/MySQL_DataBase.cpp
)
add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE 
                        ${SQLITE3_LIBRARIES} 
                        ${SODIUM_LIBRARIES} 
                        ${MYSQL_LIB}
                     )
                        
target_link_libraries(main PRIVATE 
                        ${SQLITE3_LIBRARIES} 
                        ${SODIUM_LIBRARIES}
                        ${MYSQL_LIB}
                     )
target_include_directories(main PRIVATE ${SQLITE3_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS} include)

# --- 4. Exécutable de Tests (Google Test) ---
# Note : On ne remet pas src/main.cpp ici
add_executable(run_tests 
    tests/test_units.cpp 
    # tests/unit_tests.cpp 
    src/Data_base.cpp
    src/Employe.cpp
    src/Administrateur.cpp
    src/Menu.cpp 
    src/Connexion.cpp 
    src/Admin_Data_Base.cpp 
    src/qrcode.c
    src/MySQL_DataBase.cpp
)

target_include_directories(run_tests PRIVATE include ${SQLITE3_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR})

# Liaison avec GTest::gtest_main (gère le main() automatiquement)
target_link_libraries(run_tests 
    PRIVATE 
    GTest::gtest_main 
    ${SQLITE3_LIBRARIES} 
    ${SODIUM_LIBRARIES}
    ${MYSQL_LIB}
    pthread
)

# --- 5. Activation des tests dans l'IDE ---
include(GoogleTest)
gtest_discover_tests(run_tests)
