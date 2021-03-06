# header files
set( GENERATORS_HEADER_BASENAMES
    KSGeneratorsMessage.h
    KSGenCreator.h

    KSGenValue.h
    KSGenValueBoltzmann.h
    KSGenValueFix.h
    KSGenValueSet.h
    KSGenValueList.h
    KSGenValueUniform.h
    KSGenValueGauss.h
    KSGenValueGeneralizedGauss.h
    KSGenValueRadiusCylindrical.h
    KSGenValueRadiusSpherical.h
    KSGenValueRadiusFraction.h
    KSGenValueAngleCosine.h
    KSGenValueAngleSpherical.h
    KSGenValuePareto.h
    KSGenValueZFrustrum.h

    KSGenEnergyComposite.h
    KSGenEnergyKryptonEvent.h
    KSGenEnergyRadonEvent.h
    KSGenEnergyLeadEvent.h
    KSGenEnergyBetaDecay.h
    KSGenEnergyRydberg.h
    KSGenNComposite.h
    KSGenLComposite.h
    KSGenLUniformMaxN.h
    KSGenLStatistical.h

    KSGenPositionSurfaceAdjustmentStep.h
    KSGenPositionRectangularComposite.h
    KSGenPositionCylindricalComposite.h
    KSGenPositionSphericalComposite.h
    KSGenPositionFrustrumComposite.h
    KSGenPositionSpaceRandom.h
    KSGenPositionSurfaceRandom.h
    KSGenPositionMeshSurfaceRandom.h
    KSGenPositionMask.h
    KSGenPositionFluxTube.h
    KSGenPositionHomogeneousFluxTube.h

    KSGenDirectionSphericalComposite.h
    KSGenDirectionSurfaceComposite.h
    KSGenMomentumRectangularComposite.h

    KSGenSpinComposite.h

    KSGenTimeComposite.h

    KSGenSpecial.h
    KSGenShakeOff.h
    KSGenConversion.h
    KSGenRelaxation.h

    KSGenGeneratorComposite.h
)

if( Kassiopeia_USE_ROOT )
    list( APPEND GENERATORS_HEADER_BASENAMES
        KSGenValueFormula.h
        KSGenValueHistogram.h
        KSGenGeneratorSimulation.h
    )
endif( Kassiopeia_USE_ROOT )

set( GENERATORS_HEADER_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)
foreach( BASENAME ${GENERATORS_HEADER_BASENAMES} )
    list( APPEND GENERATORS_HEADER_FILENAMES ${GENERATORS_HEADER_PATH}/${BASENAME} )
endforeach( BASENAME )

# source files
set( GENERATORS_SOURCE_BASENAMES
    KSGeneratorsMessage.cxx
    KSGenCreator.cxx

    KSGenValue.cxx
    KSGenValueBoltzmann.cxx
    KSGenValueFix.cxx
    KSGenValueSet.cxx
    KSGenValueList.cxx
    KSGenValueUniform.cxx
    KSGenValueGauss.cxx
    KSGenValueGeneralizedGauss.cxx
    KSGenValueRadiusCylindrical.cxx
    KSGenValueRadiusSpherical.cxx
    KSGenValueRadiusFraction.cxx
    KSGenValueAngleCosine.cxx
    KSGenValueAngleSpherical.cxx
    KSGenValuePareto.cxx
    KSGenValueZFrustrum.cxx

    KSGenEnergyComposite.cxx
    KSGenEnergyKryptonEvent.cxx
    KSGenEnergyRadonEvent.cxx
    KSGenEnergyLeadEvent.cxx
    KSGenEnergyBetaDecay.cxx
    KSGenEnergyRydberg.cxx
    KSGenNComposite.cxx
    KSGenLComposite.cxx
    KSGenLUniformMaxN.cxx
    KSGenLStatistical.cxx

    KSGenPositionSurfaceAdjustmentStep.cxx
    KSGenPositionRectangularComposite.cxx
    KSGenPositionCylindricalComposite.cxx
    KSGenPositionSphericalComposite.cxx
    KSGenPositionFrustrumComposite.cxx
    KSGenPositionSpaceRandom.cxx
    KSGenPositionSurfaceRandom.cxx
    KSGenPositionMeshSurfaceRandom.cxx
    KSGenPositionMask.cxx
    KSGenPositionFluxTube.cxx
    KSGenPositionHomogeneousFluxTube.cxx

    KSGenDirectionSphericalComposite.cxx
    KSGenDirectionSurfaceComposite.cxx

    KSGenMomentumRectangularComposite.cxx

    KSGenSpinComposite.cxx

    KSGenTimeComposite.cxx

    KSGenSpecial.cxx
    KSGenShakeOff.cxx
    KSGenConversion.cxx
    KSGenRelaxation.cxx

    KSGenGeneratorComposite.cxx
)

if( Kassiopeia_USE_ROOT )
    list( APPEND GENERATORS_SOURCE_BASENAMES
        KSGenValueFormula.cxx
        KSGenValueHistogram.cxx
        KSGenGeneratorSimulation.cxx
    )
endif( Kassiopeia_USE_ROOT )

set( GENERATORS_SOURCE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)
foreach( BASENAME ${GENERATORS_SOURCE_BASENAMES} )
    list( APPEND GENERATORS_SOURCE_FILENAMES ${GENERATORS_SOURCE_PATH}/${BASENAME} )
endforeach( BASENAME )

# data files
set( GENERATORS_DATA_BASENAMES
    ConversionRn219.dat
    ConversionRn220.dat
    ConversionPb210.dat
    ConversionKr83.dat
    RelaxationKr.dat
    RelaxationPo.dat
    RelaxationBi.dat
    ShakeOffRn.dat
)
set( GENERATORS_DATA_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Data
)
foreach( BASENAME ${GENERATORS_DATA_BASENAMES} )
    list( APPEND GENERATORS_DATA_FILENAMES ${GENERATORS_DATA_PATH}/${BASENAME} )
endforeach( BASENAME )

# library
kasper_internal_include_directories( ${GENERATORS_HEADER_PATH} ../Readers/Include )
add_library( KassiopeiaGenerators SHARED ${GENERATORS_SOURCE_FILENAMES} )
set_target_properties( KassiopeiaGenerators PROPERTIES COMPILE_FLAGS -Wno-overloaded-virtual )
target_link_libraries( KassiopeiaGenerators
    ${Kommon_LIBRARIES}
    ${KGeoBag_LIBRARIES}
    KassiopeiaUtility
    KassiopeiaObjects
    KassiopeiaOperators
    KassiopeiaReaders
)

if( Kassiopeia_USE_ROOT )
    target_link_libraries( KassiopeiaGenerators ${ROOT_LIBRARIES} )
endif( Kassiopeia_USE_ROOT )

# install
kasper_install_headers( ${GENERATORS_HEADER_FILENAMES} )
kasper_install_data( ${GENERATORS_DATA_FILENAMES} )
kasper_install_libraries( KassiopeiaGenerators )
