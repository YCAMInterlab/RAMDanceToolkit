require 'find'

license_txt = <<EOF
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
EOF

Find.find(File.expand_path('.')) do |path|
  
  next if File.directory?(path)
  next unless (File::extname(path) == '.h' || File::extname(path) == '.cpp')
  
  basename = File::basename(path)
  
  open(path, "r+") do |file|
    
    lines = file.readlines
    lines.unshift("// \n// #{basename} - RAMDanceToolkit\n#{license_txt}\n")
    file.rewind

    lines.each do |line|
      file.write line
    end
  end
end