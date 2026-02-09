cmake_minimum_required (VERSION 3.10)
project(Gestion_Employes VERSION 1.0)

# standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bibliotheques externes (sqlite3 et sodium)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
pkg_check_modules(SODIUM REQUIRED libsodium)

#liste des fichiers sources
set(SOURCES
    src/main.cpp 
    src/Administrateur.cpp
    src/Employe.cpp 
    src/Data_base.cpp 
    src/Menu.cpp 
    src/Connexion.cpp 
    src/Admin_Data_Base.cpp 
)

# Creation de l'executable
add_executable(main ${SOURCES})

#Listes des bib et des Headers
target_link_libraries(main PRIVATE ${SQLITE3_LIBRARIES} ${SODIUM_LIBRARIES})
target_include_directories(main PRIVATE ${SQLITE3_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS})
target_include_directories(main PRIVATE include)