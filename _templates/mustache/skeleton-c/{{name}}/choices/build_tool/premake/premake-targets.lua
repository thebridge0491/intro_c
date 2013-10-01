#!/usr/bin/env lua

-- Targets premake4 script.

newoption { trigger = "prefix", value = "/usr/local",
	description = "Installation prefix"}
newoption { trigger = "buildtest", value = "ON", 
	allowed = { { "ON", "Enable build test" }, { "OFF", "Disable build test" } },
	description = "Build test(s)"}

newoption { trigger = "fmts", value = "tar.gz",
	description = "Select archive format(s)"}

newaction { trigger = "help", description = "Help - displays targets",
	execute = function ()
		os.execute("gmake help")
	end
}

newaction { trigger = "all", description = "Compile the software",
	execute = function ()
		os.execute("gmake")
	end
}

if "ON" == (_OPTIONS["buildtest"] or "ON") then
	newaction { trigger = "test", description = "Test the software",
		execute = function ()
			os.execute(string.format("LD_LIBRARY_PATH=%s:lib ./ts_main %s",
				os.getenv("LD_LIBRARY_PATH") or "", 
				#_ARGS ~= 0 and table.concat(_ARGS, " ") or ""))
		end
	}
end

newaction { trigger = "uninstall", description = "Uninstall artifacts",
	execute = function ()
		local sln = solution()
		os.execute(string.format("rm -i %s/include/%s/*.*",
			_OPTIONS["prefix"] or "/usr/local", sln.name:lower()))
		os.execute(string.format("rm -i %s/lib/*%s*.*",
			_OPTIONS["prefix"] or "/usr/local", sln.name:lower()))
		os.execute(string.format("rm -i %s/lib/pkgconfig/%s.pc",
			_OPTIONS["prefix"] or "/usr/local", sln.name:lower()))
		os.execute(string.format("rm -ir %s/share/doc/%s",
			_OPTIONS["prefix"] or "/usr/local", sln.name:lower()))
		os.execute(string.format("rm -i %s/bin/*%s*",
			_OPTIONS["prefix"] or "/usr/local", sln.name:lower()))
	end
}

newaction { trigger = "install", description = "Install artifacts",
	execute = function ()
		--[=[os.copyfile("bin/myprogram", string.format("%s/bin/myprogram", 
			_OPTIONS["prefix"] or "/usr/local"))]=]
		os.execute(string.format("cp -fR bin include lib share %s", 
			_OPTIONS["prefix"] or "/usr/local"))
	end
}

newaction { trigger = "dist", description = "Create source archive(s)",
	execute = function ()
		local sln = solution()
		distdir = string.format("%s-%s", sln.name:lower(), sln.version)
		os.copyfile(sln.basedir .. "/exclude.lst", ".") ; os.mkdir(distdir)
		
		--[=[os.execute(string.format("cd %s ; zip -9 -q --exclude @%s/exclude.lst -r - ." ..
			" | unzip -od build/%s -", path.getdirectory(_WORKING_DIR),
			path.getdirectory(_WORKING_DIR), distdir))]=]
		os.execute(string.format("tar --format=posix --dereference " ..
			"--exclude-from=%s/exclude.lst -C %s -cf - . | tar -xpf - -C %s",
			path.getdirectory(_WORKING_DIR), 
			path.getdirectory(_WORKING_DIR), distdir))
		
		for _, fmt in ipairs(string.explode(_OPTIONS["fmts"] or "tar.gz", " ")) do
			if fmt == "zip" then
				os.execute(string.format("rm -f %s ; zip -9 -q -r %s %s",
					distdir .. ".zip", distdir .. ".zip", distdir))
			else
				os.execute(string.format("rm -f %s ; tar --posix -L -caf %s %s",
					distdir .. "." .. fmt, distdir .. "." .. fmt, distdir))
			end
		end
		os.rmdir(distdir)
	end
}

newaction { trigger = "doc", description = "Generate API documentation",
	execute = function ()
		local sln = solution()
		os.execute(string.format("doxygen %s/Doxyfile_*.txt", sln.basedir))
		os.execute(string.format("rm -fr share/doc/%s/html", sln.name:lower()))
		os.execute(string.format("mv -f html share/doc/%s/html", 
			sln.name:lower()))
	end
}

newaction { trigger = "lint", description = "Lint check source code",
	execute = function ()
		local lintopts = "--enable=all --report-progress --quiet --force " ..
			"--std=c99 --std=posix -Iinclude"
		os.execute(string.format("cd %s ; cppcheck %s src", 
			solution().basedir, lintopts))
	end
}

newaction { trigger = "report", description = "Report code coverage",
	execute = function ()
		os.execute(string.format("lcov --capture -d . -o .coverage " ..
			"--gcov-tool %s/llvm-gcov.sh", solution().basedir))
		os.execute("genhtml -o cov .coverage")
	end
}
