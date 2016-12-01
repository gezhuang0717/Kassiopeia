# headers
set( KOMMON_CORE_HEADER_FILENAMES
    Typelists/KTypeNull.h
    Typelists/KTypeList.h
    Typelists/KTypeChain.h
    Typelists/KTypeOperation.h
    Units/KDimensions.h
    Units/KUnits.h
    File/KFileMessage.h
    File/KFile.h
    File/KTextFile.h
    Initialization/KInitializationMessage.hh
    Initialization/KToken.hh
    Initialization/KTypedTokens.hh
    Initialization/KProcessor.hh
    Initialization/KCommandLineTokenizer.hh
    Initialization/KXMLTokenizer.hh
    Initialization/KChattyProcessor.hh
    Initialization/KSaveSettingsProcessor.hh
    Initialization/KVariableProcessor.hh
    Initialization/KIncludeProcessor.hh
    Initialization/KLoopProcessor.hh
    Initialization/KConditionProcessor.hh
    Initialization/KSerializationProcessor.hh
    Initialization/KPrintProcessor.hh
    Initialization/KTagProcessor.hh
    Initialization/KContainer.hh
    Initialization/KAttributeBase.hh
    Initialization/KAttribute.hh
    Initialization/KElementBase.hh
    Initialization/KSimpleElement.hh
    Initialization/KComplexElement.hh
    Initialization/KElementProcessor.hh
    Utility/Gnuplot.hpp
    Utility/KException.h
    Utility/KRandom.h
    Utility/KConst.h
    Utility/KConsoleMuter.h
    Utility/KHash.h
    Utility/KHistogram.h
    Utility/KMathConvolution.h
    Utility/KMathGamma.h
    Utility/KMathKahanSum.h
    Utility/KMathLagrangeInterpolator.h
    Utility/KMathIntegrator.h
    Utility/KMathIntegrator2D.h
    Utility/KMathIntegratorFerenc.h
    Utility/KMathLagrangeInterpolator.h
    Utility/KMathOperands.h
    Utility/KMathRegulaFalsi.h
    Utility/KMathShepardInterpolator.h
    Utility/KNonCopyable.h
    Utility/KNumeric.h
    Utility/KField.h
    Utility/KInitializer.h
    Utility/KSingleton.h
    Utility/KSmartPointer.h
    Utility/KTypeTraits.h
    Utility/KMessage.h
    Utility/KMessageBuilder.h
    Utility/KUtilityMessage.h
    Utility/KNamed.h
    Utility/KTagged.h
    Utility/KWindow.h
    Utility/KPainter.h
    Utility/KMathBilinearInterpolator.h
    Utility/KHashMap.h
    Logging/KLogger.h
)

# sources
set( KOMMON_CORE_SOURCE_FILENAMES
    File/KFileMessage.cxx
    File/KFile.cxx
    File/KTextFile.cxx
    Initialization/KInitializationMessage.cc
    Initialization/KToken.cc
    Initialization/KProcessor.cc
    Initialization/KXMLTokenizer.cc
    Initialization/KCommandLineTokenizer.cc
    Initialization/KChattyProcessor.cc
    Initialization/KSaveSettingsProcessor.cc
    Initialization/KVariableProcessor.cc
    Initialization/KIncludeProcessor.cc
    Initialization/KLoopProcessor.cc
    Initialization/KConditionProcessor.cc
    Initialization/KSerializationProcessor.cc
    Initialization/KPrintProcessor.cc
    Initialization/KTagProcessor.cc
    Initialization/KContainer.cc
    Initialization/KAttributeBase.cc
    Initialization/KElementBase.cc
    Initialization/KElementProcessor.cc
    Utility/Gnuplot.cpp
    Utility/KMessage.cxx
    Utility/KMessageBuilder.cxx
    Utility/KUtilityMessage.cxx
    Utility/KNamed.cxx
    Utility/KTagged.cxx
    Utility/KWindow.cxx
    Utility/KPainter.cxx
    Logging/KLogger.cxx
)

# external
option( Kommon_USE_Log4CXX "Enable advanced logging facilities, if LOG4CXX library is available." ON )
set(LOG4CXX_FOUND FALSE)

if(Kommon_USE_Log4CXX)
    find_package( Log4CXX )
endif()

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "DEBUG" OR CMAKE_BUILD_TYPE MATCHES "RELWITHDEBINFO" )
    set (LEVEL "DEBUG")
else()
    set (LEVEL "INFO")
endif()

if( LOG4CXX_FOUND )
    set(LOGGER_CONFIGURATION "${Kommon_CONFIG_INSTALL_DIR}/log4cxx.properties" )
    set_source_files_properties(Logging/KLogger.cxx PROPERTIES COMPILE_DEFINITIONS
        "LOG4CXX;LOGGER_CONFIGURATION=${LOGGER_CONFIGURATION}"
    )
    
    kasper_external_include_directories( ${LOG4CXX_INCLUDE_DIR} )
    
    configure_file( Logging/log4cxx.properties.in ${CMAKE_CURRENT_BINARY_DIR}/log4cxx.properties @ONLY )
    kasper_install_optional( ${Kommon_CONFIG_INSTALL_DIR} ${CMAKE_CURRENT_BINARY_DIR}/log4cxx.properties )
endif()

# internal
kasper_internal_include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Functions
	${CMAKE_CURRENT_SOURCE_DIR}/Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/Logging
	${CMAKE_CURRENT_SOURCE_DIR}/File
	${CMAKE_CURRENT_SOURCE_DIR}/Initialization
	${CMAKE_CURRENT_SOURCE_DIR}/Typelists
	${CMAKE_CURRENT_SOURCE_DIR}/Units
	${CMAKE_CURRENT_SOURCE_DIR}/Utility
)

# link
add_library( KommonCore SHARED ${KOMMON_CORE_SOURCE_FILENAMES} )
if( LOG4CXX_FOUND )
	target_link_libraries( KommonCore ${LOG4CXX_LIBRARY} )
else()
    target_link_libraries( KommonCore )
endif()

# install
kasper_install_libraries( KommonCore )
kasper_install_headers( ${KOMMON_CORE_HEADER_FILENAMES} )