# C rules rakefile script.

class SharedVars
  attr_accessor :cppflags, :ldflags, :cflags, :asflags, :arflags, :ldlibs
end

CC = 'clang'
somajor = VARS.version.split('.')[0]
sominor = VARS.version.split('.')[1]

rule '.a' do |t| 
  sh "ar #{VARS.arflags} #{t.name} #{t.prerequisites.join(' ')}"
end
#task :symlink_shlib do
#  if "dylib" == "#{SOSUFFIX}"
#    ln_s("lib#{VARS.proj}.#{somajor}.dylib", "lib/lib#{VARS.proj}.dylib",
#      :force => true) || true
#    ln_s("lib#{VARS.proj}.#{VARS.version}.dylib",
#      "lib/lib#{VARS.proj}.#{somajor}.dylib", :force => true) || true
#  else
#    ln_s("lib#{VARS.proj}.so.#{somajor}", "lib/lib#{VARS.proj}.so",
#      :force => true) || true
#    ln_s("lib#{VARS.proj}.so.#{VARS.version}",
#      "lib/lib#{VARS.proj}.so.#{somajor}", :force => true) || true
#  end
#end
SYMLINK_SHLIB = lambda { |v, t|
  if "dylib" == "#{SOSUFFIX}"
    ln_s("{File.basename(t.name).sub(/dylib/, '#{somajor}.dylib')}",
      t.name, :force => true) || true
    ln_s("{File.basename(t.name).sub(/dylib/, '#{v.version}.dylib')}",
      "#{t.name.sub(/dylib/, '#{somajor}.dylib')}", :force => true) || true
  else
    ln_s("#{File.basename(t.name)}.#{somajor}", t.name, :force => true) || true
    ln_s("#{File.basename(t.name)}.#{v.version}", "#{t.name}.#{somajor}",
      :force => true) || true
  end
}
rule ".#{SOSUFFIX}" do |t| 
  #task(:symlink_shlib).invoke
  SYMLINK_SHLIB.call(VARS, t)
  if "dylib" == "#{SOSUFFIX}"
    sh "#{LINK_c.call(VARS, t)} -fPIC -dynamiclib -undefined suppress -flat_namespace -Wl,-install_name,@rpath/#{File.basename(t.name).sub(/dylib/, '#{somajor}.dylib')},-current_version,#{VARS.version},-compatibility_version,#{somajor}.#{sominor}.0 -o #{t.name.sub(/dylib/, '#{VARS.version}.dylib')} #{VARS.ldlibs}" || true
  else
    sh "#{LINK_c.call(VARS, t)} -fPIC -shared -Wl,-soname,#{File.basename(t.name)}.#{somajor} -o #{t.name}.#{VARS.version} #{VARS.ldlibs}" || true
  end
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
