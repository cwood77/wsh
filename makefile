COMPILE_CMD = x86_64-w64-mingw32-g++
LINK_CMD = x86_64-w64-mingw32-g++
OBJ_DIR = bin/obj
OUT_DIR = bin/out
DEBUG_CC_FLAGS = -ggdb -c -Wall -D cdwDebugMode -D cdwTest -Wno-invalid-offsetof
RELEASE_CC_FLAGS = -O3 -c -Wall -D cdwTest -Wno-invalid-offsetof
DEBUG_LNK_FLAGS_POST = -ggdb -static-libgcc -static-libstdc++ -static
RELEASE_LNK_FLAGS_POST = -static-libgcc -static-libstdc++ -static

all: \
	$(OUT_DIR)/debug/console.dll \
	$(OUT_DIR)/debug/cui.dll \
	$(OUT_DIR)/debug/ex_proc_child.exe \
	$(OUT_DIR)/debug/ex_proc_parent.exe \
	$(OUT_DIR)/debug/exec.dll \
	$(OUT_DIR)/debug/file.dll \
	$(OUT_DIR)/debug/file.test.dll \
	$(OUT_DIR)/debug/ledit.dll \
	$(OUT_DIR)/debug/ledit.test.dll \
	$(OUT_DIR)/debug/outcor.dll \
	$(OUT_DIR)/debug/outcor.test.dll \
	$(OUT_DIR)/debug/resolve.dll \
	$(OUT_DIR)/debug/tcatbin.dll \
	$(OUT_DIR)/debug/q.exe \
	$(OUT_DIR)/debug/fork.exe \
	$(OUT_DIR)/debug/childTest.exe \
	$(OUT_DIR)/debug/test.exe \
	$(OUT_DIR)/debug/wsh.exe \
	$(OUT_DIR)/release/console.dll \
	$(OUT_DIR)/release/cui.dll \
	$(OUT_DIR)/release/exec.dll \
	$(OUT_DIR)/release/file.dll \
	$(OUT_DIR)/release/file.test.dll \
	$(OUT_DIR)/release/ledit.dll \
	$(OUT_DIR)/release/ledit.test.dll \
	$(OUT_DIR)/release/outcor.dll \
	$(OUT_DIR)/release/outcor.test.dll \
	$(OUT_DIR)/release/resolve.dll \
	$(OUT_DIR)/release/tcatbin.dll \
	$(OUT_DIR)/release/q.exe \
	$(OUT_DIR)/release/fork.exe \
	$(OUT_DIR)/release/childTest.exe \
	$(OUT_DIR)/release/test.exe \
	$(OUT_DIR)/release/wsh.exe
	$(OUT_DIR)/debug/test.exe
	$(OUT_DIR)/release/test.exe

clean:
	rm -rf bin
	rm -rf gen

.PHONY: all clean

# ----------------------------------------------------------------------
# tcatlib

TCATLIB_SRC = src/tcatlib/api.cpp
TCATLIB_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TCATLIB_SRC)))

$(OUT_DIR)/debug/tcatlib.lib: $(TCATLIB_DEBUG_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@ar crs $@ $<

$(TCATLIB_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/tcatlib
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TCATLIB_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TCATLIB_SRC)))

$(OUT_DIR)/release/tcatlib.lib: $(TCATLIB_RELEASE_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@ar crs $@ $<

$(TCATLIB_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/tcatlib
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# tcatbin

TCATBIN_SRC = \
	src/tcatbin/impl.cpp \
	src/tcatbin/metadata.cpp \
	src/tcatbin/tables.cpp \

TCATBIN_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TCATBIN_SRC)))

$(OUT_DIR)/debug/tcatbin.dll: $(TCATBIN_DEBUG_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(TCATBIN_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST)

$(TCATBIN_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/tcatbin
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TCATBIN_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TCATBIN_SRC)))

$(OUT_DIR)/release/tcatbin.dll: $(TCATBIN_RELEASE_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(TCATBIN_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST)

$(TCATBIN_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/tcatbin
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# console

CONSOLE_SRC = \
	src/console/commandLineParser.cpp \
	src/console/log.cpp \

CONSOLE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CONSOLE_SRC)))

$(OUT_DIR)/debug/console.dll: $(CONSOLE_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(CONSOLE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(CONSOLE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/console
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CONSOLE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CONSOLE_SRC)))

$(OUT_DIR)/release/console.dll: $(CONSOLE_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(CONSOLE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(CONSOLE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/console
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# exec

EXEC_SRC = \
	src/exec/cancel.cpp \
	src/exec/scriptRunner.cpp \

EXEC_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(EXEC_SRC)))

$(OUT_DIR)/debug/exec.dll: $(EXEC_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(EXEC_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(EXEC_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/exec
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

EXEC_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(EXEC_SRC)))

$(OUT_DIR)/release/exec.dll: $(EXEC_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(EXEC_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(EXEC_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/exec
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# file

FILE_SRC = \
	src/file/api.cpp \
	src/file/manager.cpp \
	src/file/parse.cpp \
	src/file/parse.test.cpp \

FILE_TEST_SRC = \
	src/file/sst.test.cpp \

FILE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(FILE_SRC)))

$(OUT_DIR)/debug/file.dll: $(FILE_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(FILE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(FILE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/file
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

FILE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(FILE_SRC)))

$(OUT_DIR)/release/file.dll: $(FILE_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(FILE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(FILE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/file
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

FILE_TEST_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(FILE_TEST_SRC)))

$(OUT_DIR)/debug/file.test.dll: $(FILE_TEST_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(FILE_TEST_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(FILE_TEST_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/file.test
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

FILE_TEST_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(FILE_TEST_SRC)))

$(OUT_DIR)/release/file.test.dll: $(FILE_TEST_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(FILE_TEST_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(FILE_TEST_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/file.test
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# pen

PEN_SRC = \
	src/cui/pen.cpp \

PEN_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(PEN_SRC)))

$(OUT_DIR)/debug/pen.lib: $(PEN_DEBUG_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@ar crs $@ $(PEN_DEBUG_OBJ)

$(PEN_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/cui
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

PEN_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(PEN_SRC)))

$(OUT_DIR)/release/pen.lib: $(PEN_RELEASE_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@ar crs $@ $(PEN_RELEASE_OBJ)

$(PEN_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/cui
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# cui

CUI_SRC = \
	src/cui/api.cpp \

CUI_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CUI_SRC)))

$(OUT_DIR)/debug/cui.dll: $(CUI_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(CUI_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lpen

$(CUI_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/cui
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CUI_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CUI_SRC)))

$(OUT_DIR)/release/cui.dll: $(CUI_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(CUI_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lpen

$(CUI_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/cui
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# ledit

LEDIT_SRC = \
	src/ledit/api.cpp \
	src/ledit/basic.cpp \
	src/ledit/file.cpp \
	src/ledit/history.cpp \
	src/ledit/help.cpp \
	src/ledit/printer.cpp \
	src/ledit/splitter.cpp \

LEDIT_TEST_SRC = \
	src/ledit/api.test.cpp \

LEDIT_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(LEDIT_SRC)))

$(OUT_DIR)/debug/ledit.dll: $(LEDIT_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(LEDIT_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lpen

$(LEDIT_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/ledit
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

LEDIT_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(LEDIT_SRC)))

$(OUT_DIR)/release/ledit.dll: $(LEDIT_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(LEDIT_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lpen

$(LEDIT_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/ledit
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

LEDIT_TEST_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(LEDIT_TEST_SRC)))

$(OUT_DIR)/debug/ledit.test.dll: $(LEDIT_TEST_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(LEDIT_TEST_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(LEDIT_TEST_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/ledit.test
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

LEDIT_TEST_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(LEDIT_TEST_SRC)))

$(OUT_DIR)/release/ledit.test.dll: $(LEDIT_TEST_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(LEDIT_TEST_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(LEDIT_TEST_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/ledit.test
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# outcor

OUTCOR_SRC = \
	src/outcor/color.cpp \
	src/outcor/facade.cpp \
	src/outcor/outcor.cpp \
	src/q/tailCmd.cpp \

OUTCOR_TEST_SRC = \
	src/outcor/facade.test.cpp \

OUTCOR_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(OUTCOR_SRC)))

$(OUT_DIR)/debug/outcor.dll: $(OUTCOR_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(OUTCOR_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lpen

$(OUTCOR_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/outcor
	@mkdir -p $(OBJ_DIR)/debug/q
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

OUTCOR_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(OUTCOR_SRC)))

$(OUT_DIR)/release/outcor.dll: $(OUTCOR_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(OUTCOR_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lpen

$(OUTCOR_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/outcor
	@mkdir -p $(OBJ_DIR)/release/q
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

OUTCOR_TEST_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(OUTCOR_TEST_SRC)))

$(OUT_DIR)/debug/outcor.test.dll: $(OUTCOR_TEST_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(OUTCOR_TEST_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(OUTCOR_TEST_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/outcor.test
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

OUTCOR_TEST_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(OUTCOR_TEST_SRC)))

$(OUT_DIR)/release/outcor.test.dll: $(OUTCOR_TEST_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(OUTCOR_TEST_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(OUTCOR_TEST_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/outcor.test
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# resolve

RESOLVE_SRC = \
	src/resolve/api.cpp \

RESOLVE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(RESOLVE_SRC)))

$(OUT_DIR)/debug/resolve.dll: $(RESOLVE_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(RESOLVE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(RESOLVE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/resolve
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

RESOLVE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(RESOLVE_SRC)))

$(OUT_DIR)/release/resolve.dll: $(RESOLVE_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(RESOLVE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(RESOLVE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/resolve
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# quitcmd

QUITCMD_SRC = \
	src/q/main.cpp \

QUITCMD_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(QUITCMD_SRC)))

$(OUT_DIR)/debug/q.exe: $(QUITCMD_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(QUITCMD_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(QUITCMD_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/q
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

QUITCMD_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(QUITCMD_SRC)))

$(OUT_DIR)/release/q.exe: $(QUITCMD_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(QUITCMD_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(QUITCMD_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/q
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# forkcmd

FORKCMD_SRC = \
	src/fork/main.cpp \

FORKCMD_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(FORKCMD_SRC)))

$(OUT_DIR)/debug/fork.exe: $(FORKCMD_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(FORKCMD_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(FORKCMD_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/fork
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

FORKCMD_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(FORKCMD_SRC)))

$(OUT_DIR)/release/fork.exe: $(FORKCMD_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(FORKCMD_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(FORKCMD_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/fork
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# childTestcmd

CHILDTESTCMD_SRC = \
	src/childTest/main.cpp \

CHILDTESTCMD_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CHILDTESTCMD_SRC)))

$(OUT_DIR)/debug/childTest.exe: $(CHILDTESTCMD_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(CHILDTESTCMD_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(CHILDTESTCMD_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/childTest
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CHILDTESTCMD_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CHILDTESTCMD_SRC)))

$(OUT_DIR)/release/childTest.exe: $(CHILDTESTCMD_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(CHILDTESTCMD_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(CHILDTESTCMD_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/childTest
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# test

TEST_SRC = \
	src/test/assert.cpp \
	src/test/main.cpp \

TEST_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TEST_SRC)))

$(OUT_DIR)/debug/test.exe: $(TEST_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(TEST_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(TEST_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/test
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TEST_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TEST_SRC)))

$(OUT_DIR)/release/test.exe: $(TEST_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(TEST_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(TEST_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/test
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# wsh

WSH_SRC = \
	src/wsh/main.cpp \
	src/wsh/runLoop.cpp \
	src/wsh/verb.int.cpp \
	src/wsh/verb.keyTest.cpp \

WSH_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(WSH_SRC)))

$(OUT_DIR)/debug/wsh.exe: $(WSH_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib $(OUT_DIR)/debug/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(WSH_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib -lole32 -lpen

$(WSH_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/wsh
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

WSH_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(WSH_SRC)))

$(OUT_DIR)/release/wsh.exe: $(WSH_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib $(OUT_DIR)/release/pen.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(WSH_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib -lole32 -lpen

$(WSH_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/wsh
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# ex_proc_parent

EX_PROC_PARENT_SRC = \
	ex/proc_parent/main.cpp \

EX_PROC_PARENT_DEBUG_OBJ = $(subst ex,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(EX_PROC_PARENT_SRC)))

$(OUT_DIR)/debug/ex_proc_parent.exe: $(EX_PROC_PARENT_DEBUG_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(EX_PROC_PARENT_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug

$(EX_PROC_PARENT_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: ex/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/proc_parent
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) -DUNICODE -Wno-main $< -o $@

EX_PROC_PARENT_RELEASE_OBJ = $(subst ex,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(EX_PROC_PARENT_SRC)))

$(OUT_DIR)/release/ex_proc_parent.exe: $(EX_PROC_PARENT_RELEASE_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(EX_PROC_PARENT_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release

$(EX_PROC_PARENT_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: ex/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/proc_parent
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# ex_proc_child

EX_PROC_CHILD_SRC = \
	ex/proc_child/main.cpp \

EX_PROC_CHILD_DEBUG_OBJ = $(subst ex,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(EX_PROC_CHILD_SRC)))

$(OUT_DIR)/debug/ex_proc_child.exe: $(EX_PROC_CHILD_DEBUG_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(EX_PROC_CHILD_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug

$(EX_PROC_CHILD_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: ex/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/proc_child
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) -DUNICODE -Wno-main $< -o $@

EX_PROC_CHILD_RELEASE_OBJ = $(subst ex,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(EX_PROC_CHILD_SRC)))

$(OUT_DIR)/release/ex_proc_child.exe: $(EX_PROC_CHILD_RELEASE_OBJ)
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(EX_PROC_CHILD_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release

$(EX_PROC_CHILD_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: ex/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/proc_child
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
