//
// Created by DexrnZacAttack on 12/27/2024.
//
// "JavascriptHellUtility.cpp"


#include "libLCEExports.h"
#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

#include "Compression/Compression.h"
#include "World/Chunk.h"
#include "World/Region.h"
#include "World/SplitSave.h"
#include "Save/Thumb.h"
#include "Save/SaveFile.h"
#include "Save/SaveFileOld.h"
#include "Save/SaveFileCommons.h"
#include "Save/IndexInnerFile.h"
#include "libLCE.h"

std::vector<uint8_t> VectorFromUInt8Array(emscripten::val arr) {
    return emscripten::convertJSArrayToNumberVector<uint8_t>(arr);
}

emscripten::val VectorToUInt8Array(const std::vector<uint8_t>& vec) {
    emscripten::val arr = emscripten::val::global("Uint8Array").new_(vec.size());
    arr.call<void>("set", emscripten::val::array(vec.begin(), vec.end()));

    return arr;
}


EMSCRIPTEN_BINDINGS(libLCE) {
    emscripten::register_vector<lce::save::IndexInnerFile>("vector<lce::save::IndexInnerFile>");
    emscripten::register_vector<uint8_t>("vector<uint8_t>");

    emscripten::register_optional<lce::save::IndexInnerFile>();

    emscripten::function("getLibraryVersion", &getLibraryVersion, emscripten::allow_raw_pointer<const char*>());
    emscripten::function("printLibraryInfo", &printLibraryInfo);

    emscripten::function("VectorFromUInt8Array", &VectorFromUInt8Array);
    emscripten::function("VectorToUInt8Array", &VectorToUInt8Array);

    emscripten::enum_<ByteOrder>("ByteOrder")
        .value("LITTLE", ByteOrder::LITTLE)
        .value("BIG", ByteOrder::BIG);

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

    emscripten::class_<lce::save::IndexInnerFile>("IndexInnerFile")
    .constructor<uint8_t*, bool, ByteOrder>()
    .constructor<>()
    .property("name", &lce::save::IndexInnerFile::getName, &lce::save::IndexInnerFile::setName, emscripten::allow_raw_pointer<std::wstring&>())
    .property("size", &lce::save::IndexInnerFile::getSize)
    .property("offset", &lce::save::IndexInnerFile::getOffset, &lce::save::IndexInnerFile::setOffset)
    .property("timestamp", &lce::save::IndexInnerFile::getTimestamp, &lce::save::IndexInnerFile::setTimestamp)
    .property("data", &lce::save::IndexInnerFile::getDataVec, &lce::save::IndexInnerFile::setDataVec)
    .property("endian", &lce::save::IndexInnerFile::getEndian, &lce::save::IndexInnerFile::setEndian);

    emscripten::class_<lce::compression::Compression>("Compression")
    .class_function("decompressZlib", &lce::compression::Compression::decompressZlib)
    .class_function("decompressZlibWithLength", &lce::compression::Compression::decompressZlibWithLength)
    .class_function("decompressVita", &lce::compression::Compression::decompressVita)
    .class_function("getSizeFromSave", &lce::compression::Compression::getSizeFromSave);

    emscripten::class_<lce::save::SaveFileCommons>("SaveFileCommons")
        .constructor<>()
        .function("addFile", &lce::save::SaveFileCommons::addFile, emscripten::allow_raw_pointers())
        .function("removeFile", &lce::save::SaveFileCommons::removeFile)
        .function("getSize", &lce::save::SaveFileCommons::getSize)
        .function("getFileByName", &lce::save::SaveFileCommons::getFileByName)
        .function("calculateIndexOffset", &lce::save::SaveFileCommons::calculateIndexOffset)
        .function("getFilesSize", &lce::save::SaveFileCommons::getFilesSize)
        // .class_function("readAuto", &lce::save::SaveFileCommons::readAuto)
        .class_function("getVersionFromData", &lce::save::SaveFileCommons::getVersionFromData)
        .property("indexFileCount", &lce::save::SaveFileCommons::getIndexSize)
        .property("originalVersion", &lce::save::SaveFileCommons::getOriginalVersion, &lce::save::SaveFileCommons::setOriginalVersion)
        .property("version", &lce::save::SaveFileCommons::getVersion, &lce::save::SaveFileCommons::setVersion)
        .property("indexOffset", &lce::save::SaveFileCommons::getIndexOffset)
        .property("endian", &lce::save::SaveFileCommons::getEndian, &lce::save::SaveFileCommons::setEndian)
        .property("index", &lce::save::SaveFileCommons::getIndex, emscripten::readonly()); // temp fix

    emscripten::class_<lce::save::SaveFile, emscripten::base<lce::save::SaveFileCommons>>("SaveFile")
    .constructor<uint32_t, uint32_t, uint16_t, uint16_t, const std::vector<lce::save::IndexInnerFile> &>()
    .constructor<ByteOrder>()
    .constructor<std::vector<uint8_t>, ByteOrder>()
    .constructor()
    .function("create", &lce::save::SaveFile::create, emscripten::allow_raw_pointer<const uint8_t*>());

    emscripten::class_<lce::save::SaveFileOld, emscripten::base<lce::save::SaveFileCommons>>("SaveFileOld")
    .constructor<ByteOrder>()
    .constructor<std::vector<uint8_t>, ByteOrder>()
    .function("create", &lce::save::SaveFileOld::create, emscripten::allow_raw_pointer<const uint8_t*>())
    .function("migrate", &lce::save::SaveFileOld::migrate, emscripten::allow_raw_pointer<lce::save::SaveFile*>());

    emscripten::class_<lce::save::Thumb>("Thumb")
    .constructor<std::vector<uint8_t>, ByteOrder, int, bool>()
    .property("worldName", &lce::save::Thumb::getWorldName, &lce::save::Thumb::setWorldName)
    .property("image", &lce::save::Thumb::getImage, &lce::save::Thumb::setImage)
    .property("properties", &lce::save::Thumb::getProperties, &lce::save::Thumb::setProperties);

    emscripten::class_<lce::world::Region>("Region")
    .constructor<std::vector<uint8_t>, std::wstring, lce::compression::CompressionType, ByteOrder>()
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
