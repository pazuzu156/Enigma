-----------------------------------------
-- Load & Configure UI (Character Select)
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

local root = winMgr:loadWindowLayout("character_select.layout")
guiSystem:setGUISheet(root)