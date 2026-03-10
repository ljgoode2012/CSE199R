# Unit Test Suite

This directory contains unit tests for the camera application models using Qt's QTest framework.

## Test Files

- **test_FileManager.cpp** - Tests for FileManager (file path generation, directory management)
- **test_CameraModel.cpp** - Tests for CameraModel (effects, observer pattern, camera state)

## Running the Tests

### Option 1: Using CTest (Recommended)

From the build directory, run all tests:

```bash
cd build
ctest -C Debug --output-on-failure
```

To run a specific test:

```bash
ctest -C Debug -R FileManagerTests --output-on-failure
ctest -C Debug -R CameraModelTests --output-on-failure
```

### Option 2: Running Test Executables Directly

```bash
# From build/Debug directory
./test_FileManager.exe
./test_CameraModel.exe
```

### Option 3: From Visual Studio

After rebuilding the project, test executables appear in the build output:
- `build/Debug/test_FileManager.exe`
- `build/Debug/test_CameraModel.exe`

Right-click on any CMake test target and select "Run Tests" in Visual Studio.

## Test Coverage

### FileManager Tests (7 tests)
- `test_getVideoDirectory()` - Verifies video directory path is valid
- `test_getPhotoDirectory()` - Verifies photo directory path is valid
- `test_getNextVideoFilePath()` - Verifies video filenames end with .mp4 and contain "video_"
- `test_getNextPhotoFilePath()` - Verifies photo filenames end with .png and contain "photo_"
- `test_uniqueFilePaths()` - Verifies generated paths are unique with timestamps

### CameraModel Tests (10 tests)
- `test_cameraModelCreation()` - Verifies model initializes correctly
- `test_applyPixelation()` - Tests pixelation effect application
- `test_applyNegative()` - Tests negative effect application
- `test_applyGreyscale()` - Tests greyscale effect application
- `test_subscribeToVideo()` - Tests observer registration for video frames
- `test_subscribeToRecording()` - Tests observer registration for recording state
- `test_unsubscribeFromVideo()` - Tests observer unsubscription from video
- `test_unsubscribeFromRecording()` - Tests observer unsubscription from recording

## Notes

- Tests use Qt Test framework for assertions and test infrastructure
- FileManager tests include timed delays to ensure unique timestamp-based filenames
- CameraModel tests verify business logic without requiring actual camera hardware
- All tests use mock observer classes for testing the observer pattern

## Building Tests

Tests are automatically built when you build the project:

```bash
cd build
cmake ..
cmake --build . --config Debug
```

Or build only tests:

```bash
cmake --build . --config Debug --target test_FileManager
cmake --build . --config Debug --target test_CameraModel
```
