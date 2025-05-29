//
// Created by DexrnZacAttack on 12/27/2024.
//
// "JavascriptHellUtility.cpp"


#include "../include/libLCEExports.h"
#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

#include <Compression/Compression.h>
#include <Localization/LocalizationFile.h>
#include <Save/SaveFile.h>
#include <Save/SaveFileCommons.h>
#include <Save/SaveFileOld.h>
#include <Save/Thumb.h>
#include <Soundbank/Soundbank.h>
#include <World/Chunk.h>
#include <World/Region.h>
#include <World/SplitSave.h>
#include <libLCE.h>

std::vector<uint8_t> VectorFromUInt8Array(emscripten::val arr) {
    return emscripten::convertJSArrayToNumberVector<uint8_t>(arr);
}

emscripten::val VectorToUInt8Array(const std::vector<uint8_t>& vec) {
    emscripten::val arr = emscripten::val::global("Uint8Array").new_(vec.size());
    arr.call<void>("set", emscripten::val::array(vec.begin(), vec.end()));

    return arr;
}


EMSCRIPTEN_BINDINGS(libLCE) {
    // one of these will work LMAO
    emscripten::register_vector<uint8_t>("vector<uint8_t>");
    emscripten::register_vector<int16_t>("vector<int16_t>");
    emscripten::register_vector<uint16_t>("vector<uint16_t>");
    emscripten::register_vector<uint32_t>("vector<uint32_t>");
    emscripten::register_vector<std::string>("vector<string>");
    emscripten::register_map<uint32_t, std::string>("map<uint32_t, string>");
    emscripten::register_map<int16_t, std::string>("map<int16_t, string>");
    emscripten::register_map<int16_t, int16_t>("map<int16_t, int16_t>");
    emscripten::register_map<uint16_t, std::string>("map<uint16_t, string>");

    emscripten::function("getLibraryVersion", &getLibraryVersion, emscripten::allow_raw_pointer<const char*>());
    emscripten::function("printLibraryInfo", &printLibraryInfo);

    emscripten::function("VectorFromUInt8Array", &VectorFromUInt8Array);
    emscripten::function("VectorToUInt8Array", &VectorToUInt8Array);

    emscripten::enum_<ByteOrder>("ByteOrder").value("LITTLE", io::ByteOrder::LITTLE).value("BIG", io::ByteOrder::BIG);

    emscripten::enum_<lce::compression::CompressionType>("CompressionType")
        .value("ZLIB", lce::compression::CompressionType::ZLIB)
        .value("LZX", lce::compression::CompressionType::LZX)
        .value("SPLIT_SAVE", lce::compression::CompressionType::SPLIT_SAVE)
        .value("CHUNK", lce::compression::CompressionType::CHUNK)
        .value("VITA", lce::compression::CompressionType::VITA)
        .value("DEFLATE", lce::compression::CompressionType::DEFLATE);

    emscripten::enum_<lce::save::SaveFileVersion>("SaveFileVersion")
        .value("PR", lce::save::SaveFileVersion::PR)
        .value("TU0054", lce::save::SaveFileVersion::TU0054)
        .value("TU5", lce::save::SaveFileVersion::TU5)
        .value("TU9", lce::save::SaveFileVersion::TU9)
        .value("TU14", lce::save::SaveFileVersion::TU14)
        .value("TU17", lce::save::SaveFileVersion::TU17)
        .value("TU19", lce::save::SaveFileVersion::TU19)
        .value("TU36", lce::save::SaveFileVersion::TU36)
        .value("TU69", lce::save::SaveFileVersion::TU69);

    emscripten::enum_<lce::msscmp::Soundbank::Generation>("Generation")
        .value("NEW_GEN", lce::msscmp::Soundbank::Generation::NEW_GEN)
        .value("OLD_GEN", lce::msscmp::Soundbank::Generation::OLD_GEN);


    emscripten::class_<lce::compression::Compression>("Compression")
        .class_function("decompressZlib", &lce::compression::Compression::decompressZlib)
        .class_function("decompressZlibWithLength", &lce::compression::Compression::decompressZlibWithLength)
        .class_function("decompressVita", &lce::compression::Compression::decompressVita)
        .class_function("getSizeFromSave", &lce::compression::Compression::getSizeFromSave);

    emscripten::class_<lce::fs::FSObject>("FSObject")
        .property("name", &lce::fs::FSObject::getName)
        .property("modifiedTime", &lce::fs::FSObject::getModifiedTimestamp)
        .property("creationTime", &lce::fs::FSObject::getCreationTimestamp)
        .property("path", &lce::fs::FSObject::getPath);

    emscripten::class_<lce::fs::File, emscripten::base<lce::fs::FSObject>>("File")
        .constructor<std::wstring, std::vector<uint8_t>, lce::fs::Directory*>(emscripten::allow_raw_pointers());
    //.property("isFile", &lce::fs::File::isFile)
    //.property("size", &lce::fs::File::getSize)
    //.property("data", &lce::fs::File::getData);

    emscripten::class_<lce::fs::Directory, emscripten::base<lce::fs::FSObject>>("Directory")
        .constructor<std::wstring, lce::fs::Directory*>(emscripten::allow_raw_pointers())
        .function("createFile", &lce::fs::Directory::createFile, emscripten::allow_raw_pointers())
        .function("createDirectory", &lce::fs::Directory::createDirectory, emscripten::allow_raw_pointers())
        .function("getChild", &lce::fs::Directory::getChild, emscripten::allow_raw_pointers())
        .function("addChild", &lce::fs::Directory::addChild)
        .function("takeChild", &lce::fs::Directory::takeChild)
        .function("removeChild", &lce::fs::Directory::removeChild)
        .function("renameChild", &lce::fs::Directory::renameChild)
        .function("moveChild", &lce::fs::Directory::moveChild, emscripten::allow_raw_pointers())
        .function("printListing", &lce::fs::Directory::printListing)
        .property("fileCount", &lce::fs::Directory::getFileCount)
        .property("directoryCount", &lce::fs::Directory::getDirectoryCount)
        .property("size", &lce::fs::Directory::getSize)
        // .property("children", &lce::fs::Directory::getChildren, emscripten::allow_raw_pointers()) // error
        .property("isFile", &lce::fs::Directory::isFile);

    emscripten::class_<lce::fs::Filesystem>("Filesystem")
        .constructor<>()
        .function("createFileRecursive", &lce::fs::Filesystem::createFileRecursive, emscripten::allow_raw_pointers())
        .function("getByPath", &lce::fs::Filesystem::getByPath, emscripten::allow_raw_pointers())
        .function("getOrCreateDirByPath", &lce::fs::Filesystem::getOrCreateDirByPath, emscripten::allow_raw_pointers())
        // .class_function("windowsToUnixDelimiter", &lce::fs::Filesystem::windowsToUnixDelimiter) emscripten doesn't
        // like these... nice .class_function("unixToWindowsDelimiter", &lce::fs::Filesystem::unixToWindowsDelimiter)
        .property("root", &lce::fs::Filesystem::getRoot, emscripten::allow_raw_pointers());

    emscripten::class_<lce::msscmp::BinkaFile, emscripten::base<lce::fs::File>>("BinkaFile")
        // .constructor<std::wstring, std::vector<uint8_t>, uint32_t, lce::fs::Directory
        // *>(emscripten::allow_raw_pointers()) // error
        .property("sampleRate", &lce::msscmp::BinkaFile::getSampleRate, &lce::msscmp::BinkaFile::setSampleRate);

    emscripten::class_<lce::msscmp::Soundbank, emscripten::base<lce::fs::Filesystem>>("Soundbank")
        // .constructor<uint8_t*>(emscripten::allow_raw_pointers()) // error
        .function("toData", &lce::msscmp::Soundbank::toData, emscripten::allow_raw_pointers());

    emscripten::class_<lce::save::SaveFileCommons, emscripten::base<lce::fs::Filesystem>>("SaveFileCommons")
        // .constructor<>()
        .function("getSize", &lce::save::SaveFileCommons::getSize)
        .function("calculateIndexOffset", &lce::save::SaveFileCommons::calculateIndexOffset)
        .class_function("getVersionFromData", &lce::save::SaveFileCommons::getVersionFromData)
        .property("originalVersion", &lce::save::SaveFileCommons::getOriginalVersion,
                  &lce::save::SaveFileCommons::setOriginalVersion)
        .property("version", &lce::save::SaveFileCommons::getVersion, &lce::save::SaveFileCommons::setVersion)
        .property("endian", &lce::save::SaveFileCommons::getEndian, &lce::save::SaveFileCommons::setEndian);


    emscripten::class_<lce::save::SaveFile, emscripten::base<lce::save::SaveFileCommons>>("SaveFile")
        .constructor<uint32_t, uint16_t, uint16_t>()
        .constructor<ByteOrder>()
        .constructor<std::vector<uint8_t>, io::ByteOrder>()
        .constructor()
        .function("toData", &lce::save::SaveFile::toData, emscripten::allow_raw_pointer<const uint8_t*>());

    emscripten::class_<lce::save::SaveFileOld, emscripten::base<lce::save::SaveFileCommons>>("SaveFileOld")
        .constructor<ByteOrder>()
        .constructor<std::vector<uint8_t>, io::ByteOrder>()
        .function("toData", &lce::save::SaveFileOld::toData, emscripten::allow_raw_pointer<const uint8_t*>())
        .function("upgrade", &lce::save::SaveFileOld::upgrade, emscripten::allow_raw_pointer<lce::save::SaveFile*>());

    // TODO: make emscripten friendly
    emscripten::class_<lce::loc::Language>("Language")
        .constructor<lce::io::BinaryIO&>()
        .constructor<uint8_t, uint32_t, std::string, uint32_t, std::vector<std::string>>()
        .function("toData", &lce::loc::Language::toData, emscripten::allow_raw_pointers())
        .property("size", &lce::loc::Language::getSize);

    // TODO: make emscripten friendly
    emscripten::class_<lce::loc::LocalizationFile>("LocalizationFile")
        .constructor<>()
        // .constructor<uint8_t*>()
        .constructor<uint32_t, uint32_t, std::unordered_map<uint32_t, std::string>, std::vector<lce::loc::Language>>()
        .constructor<uint32_t, uint32_t, std::vector<uint32_t>, std::unordered_map<uint32_t, std::string>,
                     std::vector<lce::loc::Language>>()
        .function("toData", &lce::loc::LocalizationFile::toData, emscripten::allow_raw_pointers())
        .property("size", &lce::loc::LocalizationFile::getSize);

    emscripten::class_<lce::save::Thumb>("Thumb")
        .constructor<std::vector<uint8_t>, io::ByteOrder, int, bool>()
        .property("worldName", &lce::save::Thumb::getWorldName, &lce::save::Thumb::setWorldName)
        .property("image", &lce::save::Thumb::getImage, &lce::save::Thumb::setImage)
        .property("properties", &lce::save::Thumb::getProperties, &lce::save::Thumb::setProperties);

    emscripten::class_<lce::world::Region>("Region")
        .constructor<std::vector<uint8_t>, std::wstring, lce::compression::CompressionType, io::ByteOrder>()
        .property("x", &lce::world::Region::getX, &lce::world::Region::setX)
        .property("z", &lce::world::Region::getZ, &lce::world::Region::setZ)
        .property("dim", &lce::world::Region::getDim, &lce::world::Region::setDim)
        .class_function("getXZFromFilename", &lce::world::Region::getXZFromFilename)
        .class_function("getDimFromFilename", &lce::world::Region::getDimFromFilename);

    emscripten::class_<lce::world::SplitSave, emscripten::base<lce::world::Region>>("SplitSave")
        .class_function("getXZFromFilename", &lce::world::SplitSave::getXZFromFilename)
        .class_function("getDimFromFilename", &lce::world::SplitSave::getDimFromFilename);

    emscripten::class_<lce::world::Chunk>("Chunk")
        .property("x", &lce::world::Chunk::getX)
        .property("z", &lce::world::Chunk::getZ)
        .property("version", &lce::world::Chunk::getVersion)
        .property("inhabitedTime", &lce::world::Chunk::getInhabitedTime)
        .property("lastUpdate", &lce::world::Chunk::getLastUpdate);
}
#endif
