# C rules rakefile script.

class SharedVars
  attr_accessor :cppflags, :ldflags, :cflags, :asflags, :arflags, :ldlibs
end

if 'Darwin' == `sh -c 'uname -s 2>/dev/null || echo not'`.chomp
  SHLIBEXT = 'dylib'
else
  SHLIBEXT = 'so'
  ENV['LDFLAGS'] = "#{ENV['LDFLAGS']} -Wl,--enable-new-dtags"
end

if '1' == DEBUG
  ENV['CPPFLAGS'] = "#{ENV['CPPFLAGS']} -DDEBUG -UNDEBUG "
  ENV['LDFLAGS'] = "#{ENV['LDFLAGS']} --coverage "
  ENV['CFLAGS'] = "#{ENV['CFLAGS']} -g3 -O0 --coverage "
else
  ENV['CPPFLAGS'] = "#{ENV['CPPFLAGS']} -DNDEBUG -UDEBUG "
  ENV['CFLAGS'] = "#{ENV['CFLAGS']} -O3 "
end

CC = 'clang'
ver_major = VARS.version.split('.')[0]

rule '.a' do |t| 
  sh "ar #{VARS.arflags} #{t.name} #{t.prerequisites.join(' ')}"
end
rule '.so' do |t| 
  sh "#{LINK_c.call(VARS, t)} -fPIC -shared -Wl,-soname,#{File.basename(t.name)}.#{ver_major} -o #{t.name}.#{VARS.version} #{VARS.ldlibs}" || true
  symlink("#{File.basename(t.name)}.#{VARS.version}", "#{t.name}.#{ver_major}",
    :force => true) || true
  symlink("#{File.basename(t.name)}.#{VARS.version}", "#{t.name}", :force => true) || true
end
rule '.dylib' do |t| 
  sh "#{LINK_c.call(VARS, t)} -fPIC -dynamiclib -undefined suppress -flat_namespace -Wl,-install_name,@rpath/#{File.basename(t.name).sub(/dylib/, '#{ver_major}.dylib')},-current_version,#{VARS.version},-compatibility_version,#{VARS.version} -o #{t.name.sub(/dylib/, '#{VARS.version}.dylib')} #{VARS.ldlibs}" || true
  symlink("#{t.name.sub(/dylib/, '#{VARS.version}.dylib')}", "#{t.name.sub(/dylib/, '#{ver_major}.dylib')}", :force => true) || true
  symlink("#{t.name.sub(/dylib/, '#{VARS.version}.dylib')}", "#{t.name}", :force => true) || true
end

LINK_o = lambda { |v, t| 
  "#{CC} #{v.ldflags} #{t.prerequisites.join(' ')}" }
LINK_c = lambda { |v, t| 
  "#{CC} #{v.cppflags} #{v.ldflags} #{v.cflags} #{t.prerequisites.join(' ')}" }
LINK_s = lambda { |v, t| 
  "#{CC} #{v.cppflags} #{v.ldflags} #{v.cflags} -Wa,-Iinclude,-I../src -g0 #{t.prerequisites.join(' ')}" }

rule(/\.o$/ => [lambda { |src|              # rule '.o' => '.c'
    '../' + src.sub(/\.[^.]+$/, '.c') } ]) do |t|
  sh "#{CC} #{VARS.cppflags} #{VARS.cflags} -c -o #{t.name} #{t.source}"
end
rule(/\.s$/ => [lambda { |src|              # rule '.s' => '.c'
    '../' + src.sub(/\.[^.]+$/, '.c') } ]) do |t|
  sh "#{CC} #{VARS.cppflags} #{VARS.cflags} -S -masm=intel #{t.source} -o #{t.name}.intel"
  sh "#{CC} #{VARS.cppflags} #{VARS.cflags} -S -masm=att #{t.source} -o #{t.name}"
end
rule(/\.o$/ => [lambda { |src|              # rule '.o' => '.s'
    src.sub(/\.[^.]+$/, '.s') } ]) do |t|
  #sh "as #{VARS.asflags} -o #{t.name} #{t.source}"
  sh "#{CC} #{VARS.cppflags} #{VARS.cflags} -c -o #{t.name} #{t.source}"
end
