add_requires("benchmark")

for _, file in ipairs(os.files("**.cc")) do
  local name = path.basename(file)
  target("Benchmark" .. name)
  set_kind("binary")
  add_packages("benchmark")
  set_default(false)
  add_files(file)
  add_tests("default")
  add_deps("benchmarks")
end
