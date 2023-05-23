#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include "zip.h"
#include "unzip.h"
// #include "old_archivo_binario.h" (metodo antiguo)

int main(int argc, char const *argv[]) {
    std::cout << "------------- CPP-PORTABLE-TEST -------------" << std::endl;
    std::cout << "Ruta TEMP: " + std::filesystem::temp_directory_path().string() << std::endl;

    std::string nombreArchivo = "portable_temp.zip";
    std::string rutaCompleta = std::filesystem::temp_directory_path().string() + "portable_temp_folder";
    std::string targetExtension = ".exe";
    std::filesystem::path targetFile;

    if (!std::filesystem::exists(rutaCompleta)) {
        if (std::filesystem::create_directory(rutaCompleta)) {
            std::cout << "Carpeta creada: " << rutaCompleta << std::endl;
        } else {
            std::cerr << "No se pudo crear la carpeta: " << rutaCompleta << std::endl;
        }
    } else {
        std::cout << "La carpeta ya existe: " << rutaCompleta << std::endl;
        for (const auto& entry : std::filesystem::directory_iterator(rutaCompleta)) {
            if (entry.path().extension() == targetExtension) {
                targetFile = entry.path();
                break;  // Detiene la busqueda despues de encontrar el primer archivo
            }
        }
        if (!targetFile.empty()) {
            std::cout << "Primer archivo .exe encontrado: " << targetFile << std::endl;
            LPCWSTR wideString = targetFile.c_str();
            ShellExecute(0, 0, wideString, 0, 0, SW_SHOW);
            std::cout << "Ejecutando..." << std::endl;
            return 0;
        } else {
            std::cout << "No se encontro ningun archivo .exe en la carpeta" << std::endl;
        }
    }

    std::string zipFilePathCompleto = rutaCompleta + std::string("/") + nombreArchivo;
    std::ofstream file (zipFilePathCompleto, std::ios::binary);  // Archivo para copiar los datos

    if (file) {
        HMODULE hModule = GetModuleHandle(NULL);
        HRSRC hResource = FindResource(hModule, L"file", RT_RCDATA);
        if (hResource != NULL) {
            DWORD dwSize = SizeofResource(hModule, hResource);
            HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
            if (hLoadedResource != NULL) {
                const char* pData = static_cast<const char*>(LockResource(hLoadedResource));
                // Copia los datos del arreglo al archivo
                // file.write(reinterpret_cast<const char*>(funnyvoice_zip), funnyvoice_zip_len);
                file.write(pData, dwSize);
                file.close();
                FreeResource(hLoadedResource);
            }
        }
        // Realiza operaciones con el archivo copiado
        ///////////////////////////////////
        const char* zipFilePath = zipFilePathCompleto.c_str();
        const char* destinationFolder = rutaCompleta.c_str();

        unzFile zipFile = unzOpen(zipFilePath);
        if (zipFile == nullptr) {
            std::cout << "Error al abrir el archivo ZIP." << std::endl;
            return 1;
        }

        unz_global_info globalInfo;
        if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
            std::cout << "Error al obtener informacion global del archivo ZIP." << std::endl;
            unzClose(zipFile);
            return 1;
        }

        // ARRANCA A RECORRER EL ZIP PARA DESCOMPRIMIRLO
        char extractedFilePath[256];
        unz_file_info fileInfo;
        uLong i;
        for (i = 0; i < globalInfo.number_entry; ++i) {
            if (unzGetCurrentFileInfo(zipFile, &fileInfo, extractedFilePath, sizeof(extractedFilePath), nullptr, 0, nullptr, 0) != UNZ_OK) {
                std::cout << "Error al obtener informacion del archivo en el archivo ZIP." << std::endl;
                unzClose(zipFile);
                return 1;
            }
            std::string extractedFileName = destinationFolder + std::string("/") + std::string(extractedFilePath);
            if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                std::cout << "Error al abrir el archivo en el archivo ZIP." << std::endl;
                unzClose(zipFile);
                return 1;
            }
            if (extractedFilePath[fileInfo.size_filename - 1] == '/' || extractedFilePath[fileInfo.size_filename - 1] == '\\') {
                std::cout << "Es una carpeta: " << extractedFilePath << std::endl;
                if (!std::filesystem::exists(extractedFileName)) {
                    if (std::filesystem::create_directory(extractedFileName)) {
                        std::cout << "Carpeta creada: " << extractedFileName << std::endl;
                    }
                }
            } else {
                std::cout << "Es un archivo: " << extractedFilePath << std::endl;
                std::ofstream outputFile (extractedFileName, std::ios::binary);
                if (!outputFile) {
                    std::cout << "Error al crear el archivo de salida: " << extractedFileName << std::endl;
                    unzCloseCurrentFile(zipFile);
                    unzClose(zipFile);
                    return 1;
                }
                char buffer[8192];
                int readSize;
                do {
                    readSize = unzReadCurrentFile(zipFile, buffer, sizeof(buffer));
                    if (readSize < 0) {
                        std::cout << "Error al leer el archivo en el archivo ZIP." << std::endl;
                        outputFile.close();
                        unzCloseCurrentFile(zipFile);
                        unzClose(zipFile);
                        return 1;
                    }
                    if (readSize > 0) {
                        outputFile.write(buffer, readSize);
                    }
                } while (readSize > 0);
                outputFile.close();
                unzCloseCurrentFile(zipFile);
                std::cout << "Archivo descomprimido: " << extractedFileName << std::endl;
            }
            if (unzGoToNextFile(zipFile) != UNZ_OK) {
                std::cout << "No hay siguiente archivo en el archivo ZIP." << std::endl;
            }
        }
        // TERMINA DE RECORRER EL ZIP Y LO CIERRA
        unzClose(zipFile);
        std::cout << "Descompresion completada correctamente." << std::endl;
        ///////////////////////////////////

        for (const auto& entry : std::filesystem::directory_iterator(destinationFolder)) {
            if (entry.path().extension() == targetExtension) {
                targetFile = entry.path();
                break;  // Detiene la busqueda despues de encontrar el primer archivo
            }
        }
        if (!targetFile.empty()) {
            std::cout << "Primer archivo .exe encontrado: " << targetFile << std::endl;
            LPCWSTR wideString = targetFile.c_str();
            ShellExecute(0, 0, wideString, 0, 0, SW_SHOW);
            std::cout << "Ejecutando..." << std::endl;
        } else {
            std::cout << "No se encontro ningun archivo .exe en la carpeta" << std::endl;
        }
    }

    return 0;
}
