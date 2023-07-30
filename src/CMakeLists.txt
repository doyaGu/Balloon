configure_file(Version.h.in "${BALLOON_INCLUDE_DIR}/Balloon/Version.h")

set(BALLOON_PUBLIC_HEADERS
        ${BALLOON_INCLUDE_DIR}/Balloon/Export.h
        ${BALLOON_INCLUDE_DIR}/Balloon/Version.h
        ${BALLOON_INCLUDE_DIR}/Balloon/Defines.h

        ${BALLOON_INCLUDE_DIR}/Balloon/ModInfo.h
        ${BALLOON_INCLUDE_DIR}/Balloon/Register.h

        ${BALLOON_INCLUDE_DIR}/Balloon/IMod.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IModMetadata.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IModContainer.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IModContext.h

        ${BALLOON_INCLUDE_DIR}/Balloon/ILogger.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IConfig.h

        ${BALLOON_INCLUDE_DIR}/Balloon/IEvent.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IEventManager.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IFileSystem.h
        ${BALLOON_INCLUDE_DIR}/Balloon/IDataShare.h
        )

set(BALLOON_PRIVATE_HEADERS
        Balloon.h

        ModContext.h
        ModLoader.h
        ModRegistry.h
        ModExplorer.h
        ModCandidateFinder.h
        ModCandidateScanner.h
        ModMetadataParser.h

        ModResolver.h
        ModSolver.h

        ModMetadata.h
        ModDependency.h
        ModCandidate.h
        ModContainer.h

        InterfaceInfo.h

        Event.h
        EventManager.h

        WeakRefFlag.h

        DataShare.h
        DataStack.h
        FileSystem.h
        Logger.h
        Config.h

        Variant.h
        SemanticVersion.h

        PathUtils.h
        )

set(BALLOON_SOURCES
        Balloon.cpp
        SignalHandling.cpp

        ModContext.cpp
        ModLoader.cpp
        ModRegistry.cpp
        ModExplorer.cpp
        ModCandidateFinder.cpp
        ModCandidateScanner.cpp
        ModMetadataParser.cpp

        ModResolver.cpp
        ModSolver.cpp

        ModMetadata.cpp
        ModDependency.cpp
        ModCandidate.cpp
        ModContainer.cpp

        InterfaceInfo.cpp

        Event.cpp
        EventManager.cpp

        WeakRefFlag.cpp

        DataShare.cpp
        DataStack.cpp
        FileSystem.cpp
        Logger.cpp
        Config.cpp

        Variant.cpp
        SemanticVersion.cpp

        PathUtils.cpp

        Balloon.rc
        )

add_library(Balloon SHARED ${BALLOON_SOURCES} ${BALLOON_PUBLIC_HEADERS} ${BALLOON_PRIVATE_HEADERS})
add_backward(Balloon)

target_include_directories(Balloon PUBLIC
        $<BUILD_INTERFACE:${BALLOON_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

target_link_libraries(Balloon
        PRIVATE
        physfs-static
        yyjson
        sv
        utf8
        itoa
        )

target_compile_definitions(Balloon PRIVATE "BALLOON_EXPORTS")

set_target_properties(Balloon PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        )

install(TARGETS Balloon
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(DIRECTORY ${BALLOON_INCLUDE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        )