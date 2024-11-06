#Add sources from ui directory
idf_component_register(
    SRC_DIRS 
        "." 
        "./ui"
        "./ui/screens"
        "./ui/fonts"
        "./ui/images"
        "./app"
        "./cube_dice"
<<<<<<< HEAD
        "./lottie_player"
=======
        "./game_2048"
>>>>>>> 3d7edf1 (add 2048 game)
    INCLUDE_DIRS 
        "." 
        "./ui" 
        "./app"
        "./cube_dice"
<<<<<<< HEAD
        "./lottie_player"
=======
        "./game_2048"
>>>>>>> 3d7edf1 (add 2048 game)
    )

spiffs_create_partition_assets(
    assets_A
    "../assets"
    FLASH_IN_PROJECT
    MMAP_FILE_SUPPORT_FORMAT ".bmp,json,.png"
)
