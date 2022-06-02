/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in
 * the License.
 *
 ******************************************************************************/

#define MEMORY_MAP_FILENAME "default_memory_map.json"
#define DEVICE_MAP_FILENAME "default_device_map.json"
#define SILKSCREEN_MAP_FILENAME "default_silkscreen_map.json"

#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <include/mca.hpp>
#include <include/pcilookup.hpp>
#include <include/report.hpp>
#include <include/summary.hpp>
#include <include/utils.hpp>
#include <include/cpu_factory.hpp>
#include <include/triage.hpp>

void printHelp() {
  std::cerr << "\nUsage:\n";
  std::cerr << "  acdbafigenerator [-d][-t][-v][-h][-m memory_map_file][-p device_map_file]";
  std::cerr << "[-s silkscreen_map_file] crashdump_file\n\n";
  std::cerr << "Options:\n";
  std::cerr << "  -d\t\t\tgenerate BAFI output from all crashdump files in current folder\n";
  std::cerr << "  -t\t\t\tprint triage information\n";
  std::cerr << "  --triage\t\tprint triage information\n";
  std::cerr << "  -v\t\t\tversion information\n";
  std::cerr << "  --version\t\tversion information\n";
  std::cerr << "  -h\t\t\tprint help\n";
  std::cerr << "  --help\t\tprint help\n";
  std::cerr << "  -m\t\t\timport memory map from json file\n";
  std::cerr << "  --memory-map\t\timport memory map from json file\n";
  std::cerr << "  -p\t\t\timport device map from json file\n";
  std::cerr << "  --pcie_names\t\timport device map from json file\n";
  std::cerr << "  -s\t\t\timport silkscreen map from json file\n";
  std::cerr << "  --silkscreen_names\timport silkscreen map from json file\n";
  std::cerr << "  memory_map_file\tJSON file containing memory map ";
  std::cerr << "information\n";
  std::cerr << "  device_map_file\tJSON file containing device map ";
  std::cerr << "information\n";
  std::cerr << "  silkscreen_map_file\tJSON file containing silkscreen map ";
  std::cerr << "information\n";
  std::cerr << "  crashdump_file\tJSON file containing Autonomous Crashdump\n";
  std::cerr << "\n\nInstructions:\n";
  std::cerr << "  Step 1: In Summary TSC section, locate which TSC register has";
  std::cerr << " \"Occurred first between all TSCs\" tag.\n";
  std::cerr << "  Step 2: From the first MCERR or first IERR information in step 1, locate the";
  std::cerr << "IP and MCE bank number that asserted FirstIERR, FirstMCERR and";
  std::cerr << "RMCA_FirstMCERR (i.e. FirstMCERR=CHA1 bank 10 M2MEM 0 bank 12, UPI 0 bank 5).\n";
  std::cerr << "  Step 3: Correlate above MCE bank number with error ";
  std::cerr << "information from MCE section or find device Bus/Device/";
  std::cerr << "Function number in 'First.MCERR' section if available.\n\n";
  std::cerr << "I have cscript memory map created using pci.resource_check() ";
  std::cerr << "command or iomem.txt from Linux /proc/iomem. Can I use it in BAFI?\n";
  std::cerr << "  Yes. You can use tools/cscript_map_converter.py like this:\n\n";
  std::cerr << "     > python3 csript_map_converter.py ";
  std::cerr << "<cscript_memory_map_file_format> or <iomem.txt> [-o output_path]\n\n";
  std::cerr << "  This command will create default_memory_map.json that can be ";
  std::cerr << "later used in BAFI using -m (--memory-map) option or by ";
  std::cerr << "copying this map to proper location described in README file.\n\n";
  std::cerr << "I have cscript PCIe map created using pci.devices() ";
  std::cerr << "command or lspci command output from Linux. Can I use it in BAFI?\n";
  std::cerr << "  Yes. You can use tools/devices_log_converter.py like this:\n\n";
  std::cerr << "     > python3 devices_log_converter.py ";
  std::cerr << "<cscript_device_map_file_format> or <lspci.txt> [-o output_path]\n\n";
  std::cerr << "  This command will create default_device_map.json that can be ";
  std::cerr << "later used in BAFI using -p (--pcie_names) option.\n\n";
}

void saveDumpFile(std::string fileName, nlohmann::ordered_json report,
                  bool oneLinePrint)
{
  std::filesystem::path filePath = std::filesystem::current_path().string();
  filePath /= "/var/crashdump/json/BAFI.json";
  std::cout << "filePath " << filePath << std::endl;         
  std::ofstream file(filePath);
  if (oneLinePrint)
    file << report;
  else
    file << report.dump(4);
  file.close();
}

void collectFilesFromDir(std::vector<std::string> &files)
{
    std::regex json_file_lower(".*.json");
    std::regex json_file_capital(".*.JSON");
    std::string path = std::filesystem::current_path().string();
    std::cout << "map path " << path << std::endl;
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if (std::regex_search(entry.path().string(), json_file_lower) 
        || std::regex_search(entry.path().string(), json_file_capital))
        {
          std::string line;
          std::ifstream file;
          bool if_crashdump_file = false;
          file.open(entry.path().string());
          while (getline(file, line))
          {
            if (line.find("METADATA") != std::string::npos ||
                line.find("metadata") != std::string::npos)
                if_crashdump_file = true;
          }
          if (if_crashdump_file)
            files.push_back(entry.path().string());
        }
    }
}

int main(int argc, char *argv[]) {
  bool oneLinePrint = false;
  bool decodeMultipleDumps = false;
  bool printTriageInfo = false;
  bool deviceMapProvided = false;
  bool silkscreenMapProvided = false;
  json deviceMap = NULL;
  json silkscreenMap = NULL;
  char *triageInfo = NULL;
  char *fullOutput = NULL;
  size_t triageInfoSize = 0;
  size_t fullOutputSize = 0;
  uint8_t minNumberOfParameters = 2;
  uint8_t maxNumberOfParameters = 9;
  uint8_t binaryFileArgumentPosition = 0;
  uint8_t crashdumpFileArgumentPosition = 1;
  std::vector<std::string> files;

  if (argc < minNumberOfParameters || argc > maxNumberOfParameters)
  {
    printHelp();
    return -1;
  }

  if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
  {
    printHelp();
    return 0;
  }

  if (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version")
  {
    std::cout << "BAFI version " << VERSION << std::endl;
    std::cout << "CPU release support: \n" << RELEASED << std::endl;
    std::cout << "CPU engineering support: \n" << UNRELEASED << std::endl;
    return 0;
  }

  for (int i = 0; i < argc; i++)
  {
    if (std::string(argv[i]) == "-t" || std::string(argv[i]) == "--triage")
    {
      printTriageInfo = true;
      crashdumpFileArgumentPosition++;
    }

    if (std::string(argv[i]) == "-p" || std::string(argv[i]) == "--pcie_names")
    {
      deviceMapProvided = true;
      crashdumpFileArgumentPosition += 2;

      std::ifstream iDev(argv[i + 1]);
      if (!iDev.good())
      {
        std::cerr << "File '" << "' not found.\n";
        printHelp();
        return -2;
      }

      deviceMap = json::parse(iDev, nullptr, false);
    }

    if (std::string(argv[i]) == "-s" ||
        std::string(argv[i]) == "--silkscreen_names")
    {
      silkscreenMapProvided = true;
      crashdumpFileArgumentPosition += 2;
      printf("path %s \n", argv[i + 1]);//test line
      std::ifstream iDev(argv[i + 1]);
      if (!iDev.good())
      {
        std::cerr << "File '"
              << "' not found.\n";
        printHelp();
        return -2;
      }

      silkscreenMap = json::parse(iDev, nullptr, false);
    }

    if (std::string(argv[i]) == "-d")
    {
      decodeMultipleDumps = true;
      collectFilesFromDir(files);
    }

    if (std::string(argv[i]) == "-m" || std::string(argv[i]) == "--memory-map")
    {
      crashdumpFileArgumentPosition += 2;
      PciBdfLookup PciBdfLookup(argv[i + 1]);
    }
    else
    {
      PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
        MEMORY_MAP_FILENAME);
    }
  }

  if (!decodeMultipleDumps && argv[crashdumpFileArgumentPosition] != NULL)
  {
    files.push_back(std::string(argv[crashdumpFileArgumentPosition]));
  }

  if (files.size() == 0 && !decodeMultipleDumps)
  {
    std::cerr << "No crashdump file provided \n";
    printHelp();
    return -3;
  }
  if (!deviceMapProvided)
  {
    PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
                              DEVICE_MAP_FILENAME);
    deviceMap = PciBdfLookup.lookupMap;
  }

  if (!silkscreenMapProvided)
  {
    PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
                              SILKSCREEN_MAP_FILENAME);
    silkscreenMap = PciBdfLookup.lookupMap;
  }

  for (unsigned int it = 0; it < files.size(); it++)
  {
    // Open File
    std::ifstream i(files[it]);
    // Check if it's a regular file or a directory
    std::filesystem::path file_path(files[it]);
    if (std::filesystem::is_directory(file_path))
      continue;
    if (!i.good())
    {
      std::cerr << "File '" << files[it] << "' not found.\n";
      printHelp();
      return -3;
    }

    // Read and check proper JSON format
    json inputData = json::parse(i, nullptr, false);
    if (inputData.is_discarded())
    {
      std::cerr << "File '" << files[it] << "' is not a valid JSON file\n";
      return -4;
    }

    nlohmann::ordered_json fullReport;
    nlohmann::ordered_json triageReport;
    int32_t fullOutputStatus = -6;
    int32_t triageInfoStatus = -6;
    std::string jsonDumpStr = inputData.dump();
    const char *jsonDumpChar = jsonDumpStr.c_str();
    char *jsonDump = new char[strlen(jsonDumpChar) + 1];
    strncpy(jsonDump, jsonDumpChar, strlen(jsonDumpChar) + 1);

    if (printTriageInfo)
    {
      // Generate triage information
      triageInfoStatus = getTriageInformation(jsonDump, strlen(jsonDump),
        triageInfo, &triageInfoSize, deviceMap, silkscreenMap, true);
    }
    else
    {
      // Generate full output
      fullOutputStatus = getFullOutput(jsonDump, strlen(jsonDump),
        fullOutput, &fullOutputSize, deviceMap, silkscreenMap, true);
    }

    if (fullOutputStatus == 0)
    {
      fullReport = json::parse(fullOutput);
    }
    else if (triageInfoStatus == 0)
    {
      triageReport = json::parse(triageInfo);
    }
    else if (fullOutputStatus == -1 || triageInfoStatus == -1)
    {
      std::cerr << "No crashdump file provided \n";
      printHelp();
      return -5;
    }
    else if (fullOutputStatus == -3 || triageInfoStatus == -3)
    {
      std::cerr << "Incorrect JSON root node. Expected crash_data or no root "
                 "node.\n";
      return -6;
    }
    else if (fullOutputStatus == -4 || triageInfoStatus == -4)
    {
      std::size_t found = files[it].find_last_of("/\\");
      std::cerr << "Unrecognized CPU type. Supported CPUs: ICX, CPX, SKX. "<< std::endl;
      std::cerr << files[it].substr(found + 1) << std::endl;
      continue;
    }
    else if (fullOutputStatus == -5 || triageInfoStatus == -5)
    {
      std::cerr << "Failed to prepare report" << std::endl;
      return -7;
    }

    if (decodeMultipleDumps && !printTriageInfo)
    {
      saveDumpFile(files[it], fullReport, oneLinePrint);
    }
    else if (printTriageInfo && !decodeMultipleDumps)
    {
     // std::cout << triageReport.dump(4) << std::endl;
      saveDumpFile(files[it], triageReport, oneLinePrint);
    }
    else if (printTriageInfo && decodeMultipleDumps)
    {
      saveDumpFile(files[it], triageReport, oneLinePrint);
    }
    else
    {
      // Dump report in 'pretty printed' JSON format
      std::cout << fullReport.dump(4) << std::endl;
    }

    if (triageInfo != NULL)
      delete triageInfo;
    if (fullOutput != NULL)
      delete fullOutput;
  }
  return 0;
};
