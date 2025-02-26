add_rules("mode.debug", "mode.release")

set_warnings("all", "error")
set_languages("cxxlatest")

if is_mode("debug") then
  set_symbols("debug")
  set_optimize("none")
else
  set_symbols("hidden")
  set_optimize("fastest")
  set_strip("all")
end

add_requires("benchmark")

for _, file in ipairs(os.files("**.cc")) do
  local name = path.basename(file)
  target(name)
  set_kind("binary")
  add_packages("benchmark")
  set_default(false)
  add_files(file)
  add_tests("default")
end
