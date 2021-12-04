
# Targets rakefile script.
require 'rake/clean'
require 'rake/packagetask'

[CLEAN, CLOBBER, Rake::FileList::DEFAULT_IGNORE_PATTERNS].each{|a| a.clear}
CLEAN.include('**/*.o', '*.log', '**/.coverage')
CLOBBER.include('*', '.??*')

desc 'Help info'
task :help do
  puts "===== subproject: #{VARS.proj} =====\nHelp: #{RAKE} -C build [DEBUG=1] [task]"
  sh "#{RAKE} -T"
end

desc 'Run tests: rake test\[topt1,topt2\]'
task :test, [:topt1] => :testCompile do |t, topts|
  sh "LD_LIBRARY_PATH=#{ENV['LD_LIBRARY_PATH']}:lib tests/ts_main #{topts[:topt1]} #{topts.extras.join(' ')} || true"
end

#----------------------------------------
#LINTOPTS = "--enable=information --report-progress --quiet --force --suppress=missingIncludeSystem --std=posix --std=c99 -Iinclude -I../src"
LINTOPTS = "--enable=all --report-progress --quiet --force --std=c99 --std=posix -Iinclude -I../src"

desc 'Uninstall artifacts'
task :uninstall do
  installfiles = FileList["#{PREFIX}/lib/*#{VARS.proj}*.*",
    "#{PREFIX}/lib/pkgconfig/#{VARS.proj}.pc", "#{PREFIX}/bin/*#{VARS.proj}*",
    "#{PREFIX}/share/doc/#{VARS.proj}"]
  files_found = `find include -type f`
  installfiles.include(files_found.split("\n").map { |i| "#{PREFIX}/#{i}" })
  #rm_rf(installfiles) || true
  sh "rm -ir #{installfiles.join(' ')}" || true
end

desc 'Install artifacts'
task :install do
  filelst = FileList['bin', 'include', 'lib', 'share']
  cp_r(filelst, "#{PREFIX}") || true
  sh "set +x ; #{PKG_CONFIG} --list-all | grep #{VARS.proj}"
end

file "#{VARS.proj}-#{VARS.version}" do |p|
  mkdir_p(p.name)
  #OLDPWD = Dir.pwd
  #sh "(cd .. ; zip -9 -q -x @exclude.lst -r - . | unzip -od #{OLDPWD}/#{p.name} -)"
  sh "tar --posix -L -X ../exclude.lst -C .. -cf - . | tar -xpf - -C #{p.name}"
end
if defined? Rake::PackageTask
  Rake::PackageTask.new(VARS.proj, VARS.version) do |p|
    task("#{VARS.proj}-#{VARS.version}").invoke
    cp_r("#{VARS.proj}-#{VARS.version}", 'pkg/', :remove_destination => true) || true
    #sh "mv -f #{VARS.proj}-#{VARS.version} pkg/" || true
    
    ENV.fetch('FMTS', 'tar.gz,zip').split(',').each{|fmt|
      if p.respond_to? "need_#{fmt.tr('.', '_')}="
        p.send("need_#{fmt.tr('.', '_')}=", true)
      else
        p.need_tar_gz = true
      end
    }
    task(:package).add_description "[FMTS=#{ENV.fetch('FMTS', 'tar.gz,zip')}]"
    task(:repackage).add_description "[FMTS=#{ENV.fetch('FMTS', 'tar.gz,zip')}]"
  end
else
  desc "[FMTS=#{ENV.fetch('FMTS', 'tar.gz,zip')}] Package project distribution"
  task :package => ["#{VARS.proj}-#{VARS.version}"] do |t|
    distdir = "#{VARS.proj}-#{VARS.version}"
    
    ENV.fetch('FMTS', 'tar.gz,zip').split(',').each{|fmt|
      case fmt
      when '7z'
        rm_rf("#{distdir}.7z") || true
        sh "7za a -t7z -mx=9 #{distdir}.7z #{distdir}" || true
      when 'zip'
        rm_rf("#{distdir}.zip") || true
        sh "zip -9 -q -r #{distdir}.zip #{distdir}" || true
      else
        # tarext = `echo #{fmt} | grep -e '^tar$' -e '^tar.xz$' -e '^tar.zst$' -e '^tar.bz2$' || echo tar.gz`.chomp
        tarext = fmt.match(%r{(^tar$|^tar.xz$|^tar.zst$|^tar.bz2$)}) ? fmt : 'tar.gz'
        rm_rf("#{distdir}.#{tarext}") || true
        sh "tar --posix -L -caf #{distdir}.#{tarext} #{distdir}" || true
      end
    }
  end
end

desc 'Generate API documentation'
task :doc do
  sh "doxygen ../Doxyfile_*.txt || true"
  rm_rf("share/doc/#{VARS.proj}/html") || true
  mv('html', "share/doc/#{VARS.proj}/html", :force => true) || true
end

desc 'Lint check'
task :lint do
  sh "cppcheck #{LINTOPTS} ../src || true"
end

desc 'Report code coverage'
task :report do
  # read coverage data w/ [llvm-cov] gcov -f -b -n *.gcda
  #files_found = `find . -type f -name '*.gcda'`
  #files_found.split("\n").each { |i| sh "llvm-cov -f -b --no-output #{i}" }
  # read coverage data w/ lcov -c -d . -o .coverage ... *.gcda
  sh "lcov --capture -d . -o .coverage --gcov-tool ../llvm-gcov.sh || true"
  sh "genhtml .coverage -o cov || true"
end
