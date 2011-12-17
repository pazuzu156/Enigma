-----------------------------------------
-- Setup UI Defaults
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

schemeMgr:create("TaharezLook.scheme");
schemeMgr:create("TaharezLookWidgetAliases.scheme");
schemeMgr:create("TaharezLookWidgets.scheme");

guiSystem:setDefaultFont("DejaVuSans-10")
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow")
guiSystem:setDefaultTooltip("TaharezLook/Tooltip")
