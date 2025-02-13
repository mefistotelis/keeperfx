#include "pre_inc.h"


//#include <string>

//#include "../deps/luajit/src/lua.hpp"
#include "../deps/luajit/src/lua.h"
#include "../deps/luajit/src/lauxlib.h"
#include "../deps/luajit/src/lualib.h"

#include "lua_api.h"

#include "bflib_basics.h"
#include "bflib_fileio.h"
#include "config.h"
#include "globals.h"


#include "post_inc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lua_State *Lvl_script = NULL;


// Little error checking utility function
TbBool CheckLua(lua_State *L, int result,const char* func)
{
if (result != LUA_OK) {
        const char *message = lua_tostring(L, -1);
        ERRORLOG("Lua error in %s: %s", func, message ? message : "Unknown error");

        // Print full stack trace
        luaL_traceback(L, L, NULL, 1);
        const char *trace = lua_tostring(L, -1);
        ERRORLOG("Stack trace:\n%s", trace ? trace : "No stack trace available");
        lua_pop(L, 2); // Remove error message and traceback

        exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

void close_lua_script()
{
    if(Lvl_script)
        lua_close(Lvl_script);
    Lvl_script = NULL;
}
/*
int setLuaPath( lua_State* L, const char* path )
{
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    cur_path.append( ";" );
    cur_path.append( path );
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path.c_str() ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}
*/

TbBool open_lua_script(LevelNumber lvnum)
{
	Lvl_script = luaL_newstate();

	luaL_openlibs(Lvl_script);

	reg_host_functions(Lvl_script);
	 
    short fgroup = get_level_fgroup(lvnum);
    char* fname = prepare_file_fmtpath(fgroup, "map%05lu.lua", (unsigned long)lvnum);


    //char* foldername = prepare_file_fmtpath(fgroup, "");

	// Load and parse the Lua File
    if ( !LbFileExists(fname) )
      return false;
    //setLuaPath(Lvl_script,foldername);
	if(!CheckLua(Lvl_script, luaL_dofile(Lvl_script, fname),"script_loading"))
	{
        ERRORLOG("failed to load lua script");
        close_lua_script();
        return false;
	}



    fname = prepare_file_fmtpath(FGrp_FxData, "lua/global.lua");

	// Load and parse the Lua File
    if ( !LbFileExists(fname) )
    {
        ERRORLOG("file %s missing",fname);
        return false;
    }
    //setLuaPath(Lvl_script,foldername);
	if(!CheckLua(Lvl_script, luaL_dofile(Lvl_script, fname),"global_lua_file"))
	{
        ERRORLOG("failed to load global lua script");
        close_lua_script();
        return false;
	}
    return true;
    
}




#ifdef __cplusplus
}
#endif
