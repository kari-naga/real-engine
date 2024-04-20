require 'xcodeproj'
project_path = 'game_engine.xcodeproj'
project = Xcodeproj::Project.open(project_path)
target = project.targets.first

input_h_ref = project.new_file("src/Input.h")
input_cpp_ref = project.new_file("src/Input.cpp")

target.source_build_phase.add_file_reference(input_cpp_ref)

project.save("game_engine_new.xcodeproj")