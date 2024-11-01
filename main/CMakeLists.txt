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
    INCLUDE_DIRS 
        "." 
        "./ui" 
        "./app"
        "./cube_dice"
    )

spiffs_create_partition_assets(
    assets_A
    "../assets"
    FLASH_IN_PROJECT
    MMAP_FILE_SUPPORT_FORMAT ".bmp,json"
)
