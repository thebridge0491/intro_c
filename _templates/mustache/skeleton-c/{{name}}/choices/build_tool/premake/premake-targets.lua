#!/usr/bin/env lua

-- Targets premake script.

newoption { trigger = "prefix", value = "/usr/local",
	description = "Installation prefix"}
newoption { trigger = "buildtest", value = "ON", 
	allowed = { { "ON", "Enable build test" }, { "OFF", "Disable build test" } },
	description = "Build test(s)"}

newoption { trigger = "fmts", value = "tar.gz,zip",
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
			os.execute(string.format("LD_LIBRARY_PATH=%s:lib tests/ts_main %s",
				os.getenv("LD_LIBRARY_PATH") or "", 
				#_ARGS ~= 0 and table.concat(_ARGS, " ") or ""))
		end
	}
end

newaction { trigger = "uninstall", description = "Uninstall artifacts",
	execute = function ()
		local wsp = solution()
		os.execute(string.format("find include -type f -exec rm -i \"%s/{}\" \\;",
			_OPTIONS["prefix"] or "/usr/local"))
		os.execute(string.format("rm -i %s/lib/*%s*.*",
			_OPTIONS["prefix"] or "/usr/local", wsp.name:lower()))
		os.execute(string.format("rm -i %s/lib/pkgconfig/%s.pc",
			_OPTIONS["prefix"] or "/usr/local", wsp.name:lower()))
		os.execute(string.format("rm -ir %s/share/doc/%s",
			_OPTIONS["prefix"] or "/usr/local", wsp.name:lower()))
		os.execute(string.format("rm -i %s/bin/*%s*",
			_OPTIONS["prefix"] or "/usr/local", wsp.name:lower()))
	end
}

newaction { trigger = "install", description = "Install artifacts",
	execute = function ()
		--[=[os.copyfile("bin/myprogram", string.format("%s/bin/myprogram", 
			_OPTIONS["prefix"] or "/usr/local"))]=]
		os.execute(string.format("cp -fR bin include lib share %s", 
			_OPTIONS["prefix"] or "/usr/local"))
        os.execute(string.format("sh -xc \"pkgconf --with-path=%s/lib/pkgconfig --list-all | grep %s\"", 
			_OPTIONS["prefix"] or "/usr/local", solution().name))
	end
}

newaction { trigger = "package", description = "Package source archive(s)",
	execute = function ()
		local wsp = solution()
		distdir = string.format("%s-%s", wsp.name:lower(), wsp.version)
		os.copyfile(wsp.basedir .. "/exclude.lst", ".") ; os.mkdir(distdir)
		
		--[=[os.execute(string.format("cd %s ; zip -9 -q --exclude @%s/exclude.lst -r - ." ..
			" | unzip -od build/%s -", path.getdirectory(_WORKING_DIR),
			path.getdirectory(_WORKING_DIR), distdir))]=]
		os.execute(string.format("tar --format=posix --dereference " ..
			"--exclude-from=%s/exclude.lst -C %s -cf - . | tar -xpf - -C %s",
			path.getdirectory(_WORKING_DIR), 
			path.getdirectory(_WORKING_DIR), distdir))
		
		for _, fmt in ipairs(string.explode(_OPTIONS["fmts"] or "tar.gz,zip", ",")) do
			if fmt == "7z" then
				os.execute(string.format("rm -f %s ; 7za a -t7z -mx=9 %s %s",
					distdir .. ".7z", distdir .. ".7z", distdir))
			elseif fmt == "zip" then
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
		local wsp = solution()
		os.execute(string.format("doxygen %s/Doxyfile_*.txt", wsp.basedir))
		os.execute(string.format("rm -fr share/doc/%s/html", wsp.name:lower()))
		os.execute(string.format("mv -f html share/doc/%s/html", 
			wsp.name:lower()))
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
