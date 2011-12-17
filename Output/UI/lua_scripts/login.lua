-----------------------------------------
-- Load & Configure UI (Login)
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

local root = winMgr:loadWindowLayout("login.layout")
guiSystem:setGUISheet(root)