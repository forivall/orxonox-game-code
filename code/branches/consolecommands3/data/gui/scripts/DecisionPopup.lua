-- DecisionPopup.lua

local P = createMenuSheet("DecisionPopup")

function P.setCallback(functionPtr)
    P.functionPtr = functionPtr
end

function P.setText( text )
    winMgr:getWindow("orxonox/DecisionPopup_text"):setText( text )
end

-- events for ingamemenu
function P.button_yes(e)
    if P.functionPtr ~= nil then
        P.functionPtr(true)
    end
    hideMenuSheet("DecisionPopup")
end

function P.button_no(e)
    if P.functionPtr ~= nil then
        P.functionPtr(false)
    end
    hideMenuSheet("DecisionPopup")
end

return P

