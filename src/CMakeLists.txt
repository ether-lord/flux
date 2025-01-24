set(FLUX_LIB_NAME flux-framework)

add_library(
  ${FLUX_LIB_NAME}
  shader.cpp shader.h
  resources_manager.cpp resources_manager.h
)

target_include_directories(
  ${FLUX_LIB_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/lib/json/include/nlohmann
)

target_link_libraries(
  ${FLUX_LIB_NAME} PRIVATE
  nlohmann_json::nlohmann_json
)
