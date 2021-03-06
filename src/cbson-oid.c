#include <lauxlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "cbson.h"
#include "cbson-oid.h"
#include "cbson-util.h"

DEFINE_CHECK(OID, oid)

int cbson_oid_create(lua_State* L, const char* oid)
{
  if (strlen(oid) != 24)
  {
    lua_pushstring(L, "Invalid operand. OID should be hex-string 24-chars long");
    lua_error(L);
    return 1;
  }

  cbson_oid_t* ud = lua_newuserdata(L, sizeof(cbson_oid_t));
  strcpy(ud->oid, oid);

  luaL_getmetatable(L, OID_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

int cbson_oid_new(lua_State* L)
{
  if (lua_isuserdata(L, 1) && luaL_checkudata_ex(L, 1, OID_METATABLE))
  {
    return cbson_oid_create(L, ((cbson_oid_t*)lua_touserdata(L, 1))->oid);
  }
  else if (lua_isstring(L, 1))
  {
    return cbson_oid_create(L, lua_tostring(L, 1));
  }
  else
  {
    lua_pushstring(L, "Invalid operand. Expected 'cbson.oid' or 'string'");
    lua_error(L);
    return 0;
  }
}

int cbson_oid_tostring(lua_State* L)
{
  cbson_oid_t* a = check_cbson_oid(L, 1);

  lua_pushstring(L, a->oid);
  return 1;
}

int cbson_oid_timestamp(lua_State* L)
{
  cbson_oid_t* a = check_cbson_oid(L, 1);
  char hex[9];
  uint32_t number;

  strncpy(hex, a->oid, 8);
  hex[8] = '\n';
  number = (uint32_t)strtol(hex, NULL, 16);
  lua_pushnumber(L, number);
  return 1;
}


const struct luaL_Reg cbson_oid_meta[] = {
  {"__tostring", cbson_oid_tostring},
  {NULL, NULL}
};

const struct luaL_Reg cbson_oid_methods[] = {
  {"timestamp",   cbson_oid_timestamp},
  {NULL, NULL}
};
