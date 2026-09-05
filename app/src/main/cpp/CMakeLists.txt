cmake_minimum_required(VERSION 3.22.1)
project(CarRacingGame CXX)

# ---------------------------------------------------------------------------
# C++ Standard — C++20 required for Jolt Physics and modern game code
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Jolt Physics — pulled via FetchContent (static lib, NEON on arm64)
# Pinned to a stable release tag for reproducible builds.
# ---------------------------------------------------------------------------
include(FetchContent)

# Disable Jolt options we don't need
set(JPH_BUILD_SHARED_LIBS     OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_SAMPLES         OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_UNIT_TESTS      OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_DOCS            OFF CACHE BOOL "" FORCE)
set(JPH_USE_ASSERTS           OFF CACHE BOOL "" FORCE)  # off in all builds; enable locally if needed
set(JPH_FLOATING_POINT_EXCEPTIONS_ENABLED OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    JoltPhysics
    GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
    GIT_TAG        v5.1.0
    GIT_SHALLOW    TRUE   # only fetch the tagged commit; faster clone
)
FetchContent_MakeAvailable(JoltPhysics)

# ---------------------------------------------------------------------------
# GameActivity (provided via Prefab from the Gradle dependency)
# ---------------------------------------------------------------------------
find_package(game-activity REQUIRED CONFIG)

# ---------------------------------------------------------------------------
# Game source files
# ---------------------------------------------------------------------------
set(GAME_SOURCES
    # Entry point
    main.cpp

    # Core
    core/Application.cpp
    core/GameLoop.cpp
    core/Timer.cpp
    core/AssetManager.cpp

    # Renderer
    renderer/EGLContext.cpp
    renderer/GLRenderer.cpp
    renderer/Shader.cpp
    renderer/Mesh.cpp
    renderer/Texture.cpp
    renderer/Material.cpp
    renderer/Camera.cpp

    # Physics
    physics/PhysicsSystem.cpp
    physics/VehiclePhysics.cpp

    # Gameplay
    gameplay/Car.cpp
    gameplay/CarController.cpp
    gameplay/Track.cpp
    gameplay/Checkpoint.cpp
    gameplay/LapManager.cpp
    gameplay/RaceManager.cpp
    gameplay/ai/AIAgent.cpp

    # Input
    input/InputManager.cpp
    input/TouchInput.cpp
    input/GyroscopeInput.cpp

    # UI
    ui/UIRenderer.cpp
    ui/Font.cpp
    ui/HUD.cpp
    ui/MainMenu.cpp
    ui/PauseMenu.cpp
    ui/ResultsScreen.cpp

    # Audio
    audio/AudioManager.cpp
    audio/SoundEffect.cpp

    # Scene
    scene/SceneManager.cpp
    scene/MainMenuScene.cpp
    scene/GameScene.cpp

    # Save
    save/SaveSystem.cpp
)

# ---------------------------------------------------------------------------
# Shared library (.so) — loaded by GameActivity
# ---------------------------------------------------------------------------
add_library(racing_game SHARED ${GAME_SOURCES})

# ---------------------------------------------------------------------------
# Include paths
# ---------------------------------------------------------------------------
target_include_directories(racing_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}          # so #include "core/Log.h" works
    ${joltphysics_SOURCE_DIR}            # Jolt headers live at repo root
)

# ---------------------------------------------------------------------------
# Compile options
# ---------------------------------------------------------------------------
target_compile_options(racing_game PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    # Release optimizations (AGP passes -DCMAKE_BUILD_TYPE=Release for release builds)
    $<$<CONFIG:Release>:-O2 -fvisibility=hidden>
    $<$<CONFIG:Debug>:-O0 -g>
)

# ---------------------------------------------------------------------------
# Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(racing_game
    # Jolt Physics (static)
    Jolt

    # GameActivity + android_native_app_glue
    game-activity::game-activity

    # Android system libraries
    android       # ANativeWindow, AAssetManager
    log           # __android_log_print
    EGL           # EGL context creation
    GLESv3        # OpenGL ES 3.0
    OpenSLES      # Audio (fallback to AAudio at runtime if API >= 26)
    sensor        # ASensorManager for gyroscope (API 26+)
)

# ---------------------------------------------------------------------------
# Export android_main symbol so GameActivity can find it
# ---------------------------------------------------------------------------
set_target_properties(racing_game PROPERTIES
    CXX_VISIBILITY_PRESET   hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# android_main must be exported even with hidden visibility
target_compile_definitions(racing_game PRIVATE
    RACING_GAME_EXPORT=__attribute__\(\(visibility\(\"default\"\)\)\)
)
