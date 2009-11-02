-- InGameMenu.lua

BasicGUI = require("BasicGUI")
local P = BasicGUI:new() --inherit everything from the gui package
if _REQUIREDNAME == nil then
    InGameMenu = P
else
    _G[_REQUIREDNAME] = P
end

P.filename = "InGameMenu"
P.layoutString = "InGameMenu.layout"

function P:init()
end


-- events for ingamemenu
function P.button_quit_clicked(e)
    orxonox.CommandExecutor:execute("hideGUI InGameMenu")
    orxonox.CommandExecutor:execute("exit")
end

function P.button_mainmenu_clicked(e)
    orxonox.CommandExecutor:execute("showGUI MainMenu")
    orxonox.CommandExecutor:execute("hideGUI InGameMenu")
end

return P

