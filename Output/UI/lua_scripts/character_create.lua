-----------------------------------------
-- Load & Configure UI (Character Create)
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

local root = winMgr:loadWindowLayout("character_create.layout")
guiSystem:setGUISheet(root)