# frozen_string_literal: true

require "bundler/gem_tasks"
require "rspec/core/rake_task"
require 'yard'
require "rake/extensiontask"

RSpec::Core::RakeTask.new(:spec)

YARD::Rake::YardocTask.new do |t|
 t.files   = ['lib/**/*.rb', 'ext/**/*.c']   # optional
 t.options = ['--any', '--extra', '--opts'] # optional
 t.stats_options = ['--list-undoc']         # optional
end

task build: :compile

Rake::ExtensionTask.new("termios") do |ext|
  ext.lib_dir = "lib/terminal-io"
end

task default: %i[clobber compile spec]
