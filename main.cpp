#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include "archivo_binario.h"
#include "zip.h"
#include "unzip.h"

int main(int argc, char const *argv[]) {
    std::cout << "------------- PROBANDO -------------" << std::endl;
    std::cout << "Ruta TEMP: " + std::filesystem::temp_directory_path().string() << std::endl;

    std::string nombreArchivo = "archivo_copiado.zip";
    std::string rutaCompleta = std::filesystem::temp_directory_path().string() + "carpeta_temporal";

    if (!std::filesystem::exists(rutaCompleta)) {
        if (std::filesystem::create_directory(rutaCompleta)) {
            std::cout << "Carpeta creada: " << rutaCompleta << std::endl;
        } else {
            std::cerr << "No se pudo crear la carpeta: " << rutaCompleta << std::endl;
        }
    } else {
        std::cout << "La carpeta ya existe: " << rutaCompleta << std::endl;
    }

    std::string zipFilePathCompleto = rutaCompleta + std::string("/") + nombreArchivo;
    std::ofstream file (zipFilePathCompleto, std::ios::binary);  // Archivo para copiar los datos

    if (file) {
        // Copia los datos del arreglo al archivo
        file.write(reinterpret_cast<const char*>(funnyvoice_zip), funnyvoice_zip_len);
        file.close();
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
            std::cout << "Error al obtener información global del archivo ZIP." << std::endl;
            unzClose(zipFile);
            return 1;
        }

        char extractedFilePath[256];
        unz_file_info fileInfo;
        uLong i;

        for (i = 0; i < globalInfo.number_entry; ++i) {
            if (unzGetCurrentFileInfo(zipFile, &fileInfo, extractedFilePath, sizeof(extractedFilePath), nullptr, 0, nullptr, 0) != UNZ_OK) {
                std::cout << "Error al obtener información del archivo en el archivo ZIP." << std::endl;
                unzClose(zipFile);
                return 1;
            }

            std::string extractedFileName = destinationFolder + std::string("/") + std::string(extractedFilePath);

            if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                std::cout << "Error al abrir el archivo en el archivo ZIP." << std::endl;
                unzClose(zipFile);
                return 1;
            }

            std::ofstream outputFile (extractedFileName, std::ios::binary);
            if (!outputFile) {
                std::cout << "Error al crear el archivo de salida." << std::endl;
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

            if (unzGoToNextFile(zipFile) != UNZ_OK) {
                std::cout << "No hay siguiente archivo en el archivo ZIP." << std::endl;
            }
        }

        unzClose(zipFile);

        std::cout << "Descompresion completada correctamente." << std::endl;

        std::string targetExtension = ".exe";
        std::filesystem::path targetFile;
        for (const auto& entry : std::filesystem::directory_iterator(destinationFolder)) {
            if (entry.path().extension() == targetExtension) {
                targetFile = entry.path();
                break;  // Detiene la búsqueda después de encontrar el primer archivo
            }
        }
        if (!targetFile.empty()) {
            std::cout << "Primer archivo .exe encontrado: " << targetFile << std::endl;
        } else {
            std::cout << "No se encontró ningún archivo .exe en la carpeta" << std::endl;
        }
        ///////////////////////////////////

        // std::wstring temp = std::wstring(nombreArchivo.begin(), nombreArchivo.end());
        LPCWSTR wideString = targetFile.c_str();
        ShellExecute(0, 0, wideString, 0, 0, SW_SHOW);
    } else {
        // Error al abrir el archivo de salida
    }

    return 0;
}
