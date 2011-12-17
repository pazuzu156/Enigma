-----------------------------------------
-- Load & Configure UI (In Game)
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

local root = winMgr:loadWindowLayout("in_game_chat.layout")
guiSystem:setGUISheet(root)