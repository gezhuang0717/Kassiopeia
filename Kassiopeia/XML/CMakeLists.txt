kasper_install_config_subdir( Complete 
    Complete/Objects.xml
    Complete/Geometry.xml
    Complete/Generators.xml
    Complete/Trajectories.xml
    Complete/Interactions.xml
    Complete/Navigators.xml
    Complete/Terminators.xml
    Complete/Writers.xml 
)

kasper_configure_config_subdir( Complete
    Complete/Fields.xml
)

kasper_install_config_subdir( Validation
    Validation/TestGenerator.xml
    Validation/TestTrajectory.xml
    Validation/TestSpaceInteraction.xml
)

kasper_configure_config_subdir( Validation 
    Validation/TestField.xml
    Validation/TestDiscreteRotationalField.xml
)

kasper_configure_config_subdir( Examples 
    Examples/AnalyticSimulation.xml
    Examples/QuadrupoleTrapSimulation.xml
    Examples/DipoleTrapSimulation.xml
    Examples/ToricTrapSimulation.xml
    Examples/PhotoMultiplierTubeSimulation.xml
)