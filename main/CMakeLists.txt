idf_component_register(
    SRCS 
        "main.c"
        "Common/Com_Debug.c"  
        "Inf/Inf_BDR6120.c"
        "Inf/Inf_WTN6170.c" 
        "Inf/Inf_SC12B.c"
        "Inf/Inf_FPM383.c"
        "Dri/Dri_NVS.c"
        "App/App_IO.c"             
                    
    INCLUDE_DIRS 
        "."
        "Common"
        "Inf"
        "Dri"
        "App"
    )
