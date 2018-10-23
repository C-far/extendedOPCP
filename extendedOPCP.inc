#tryinclude <streamer>
#if !defined Streamer_IncludeFileVersion
    #error This include needs streamer plugin
#endif

#tryinclude <YSF>
#if !defined AttachDynamicObjectToPlayer
    #error This include needs YSF plugin with the AttachDynamicObjectToPlayer function
#endif

//===========================================================

#define CLICK_SOURCE_SKIN (16)

static
    bool:g_opcpFirst[MAX_PLAYERS char],
    bool:g_opcpSelect[MAX_PLAYERS char],
    STREAMER_TAG_OBJECT g_opcpObject[MAX_PLAYERS]
;

public OnPlayerConnect(playerid) {
    if(!g_opcpFirst{playerid}) {
        new 
            STREAMER_TAG_OBJECT objid = CreateDynamicObject(19201, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
        ;

        for(new i; i< 10; i++)
            SetDynamicObjectMaterial(objid, i, 0, "none", "none", 0x00000000);

        g_opcpObject[playerid] = objid;
        Streamer_SetIntData(STREAMER_TYPE_OBJECT, objid, E_STREAMER_EXTRA_ID, playerid);

        AttachDynamicObjectToPlayer(g_opcpObject[playerid], playerid, 0.0, 0.0, -0.15, 0.0, 0.0, 0.0);

        g_opcpFirst{playerid} = true;
    }

    #if defined OPCP_OnPlayerConnect
        OPCP_OnPlayerConnect(playerid);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect OPCP_OnPlayerConnect
#if defined OPCP_OnPlayerConnect
    forward OPCP_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason) {
    #pragma unused reason
    
    g_opcpFirst{playerid} = false;
    g_opcpSelect{playerid} = false;
    DestroyDynamicObject(g_opcpObject[playerid]);

    #if defined OPCP_OnPlayerDisconnect
        OPCP_OnPlayerDisconnect(playerid, reason);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect OPCP_OnPlayerDisconnect
#if defined OPCP_OnPlayerDisconnect
    forward OPCP_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerSelectDynamicObject(playerid, STREAMER_TAG_OBJECT objectid, modelid, Float:x, Float:y, Float:z) {
    if(g_opcpSelect{playerid}) {
        new 
            targetid = Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID)
        ;

        if(IsPlayerConnected(targetid)) {
            if(g_opcpObject[targetid] == objectid) {
                g_opcpSelect{playerid} = false;
                CancelEdit(playerid);
                CallLocalFunction("OnPlayerClickPlayer", "ddd", playerid, targetid, CLICK_SOURCE_SKIN);
            }
        }
        return 1; 
    }

    #if defined OPCPOnPlayerSelectDynamicObject
        OPCPOnPlayerSelectDynamicObject(playerid, objectid, modelid, x, y, z);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerSelectDynObject
    #undef OnPlayerSelectDynamicObject
#else
    #define _ALS_OnPlayerSelectDynObject
#endif

#define OnPlayerSelectDynamicObject OPCPOnPlayerSelectDynamicObject
#if defined OPCPOnPlayerSelectDynamicObject
    forward OPCPOnPlayerSelectDynamicObject(playerid, STREAMER_TAG_OBJECT objectid, modelid, Float:x, Float:y, Float:z);
#endif

stock AttachDynamicObjectToPlayerEx(STREAMER_TAG_OBJECT objectid, playerid, Float:offsetx, Float:offsety, Float:offsetz, Float:rx, Float:ry, Float:rz) {
    new
        Float:x,
        Float:y,
        Float:z;

    GetPlayerPos(playerid, x, y, z);
    SetDynamicObjectPos(objectid, x+offsetx, y+offsety, z+offsetz);
    SetDynamicObjectRot(objectid, rx, ry, rz);
}

stock SelectPlayer(playerid) {
    g_opcpSelect{playerid} = true;
    SelectObject(playerid);
    
    return true;
}

stock STREAMER_TAG_OBJECT GetObject_OPCP(playerid) {
    return g_opcpObject[playerid];
}