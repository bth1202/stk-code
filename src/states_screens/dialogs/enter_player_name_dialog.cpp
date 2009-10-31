//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "guiengine/engine.hpp"
#include "guiengine/widget.hpp"
#include "states_screens/dialogs/enter_player_name_dialog.hpp"
#include "states_screens/options_screen_players.hpp"
#include "utils/translation.hpp"

using namespace GUIEngine;
using namespace irr;
using namespace irr::core;
using namespace irr::gui;

EnterPlayerNameDialog::EnterPlayerNameDialog(const float w, const float h) :
        ModalDialog(w, h)
{
    LabelWidget* widget = new LabelWidget();
    
    //I18N: In the 'add new player' dialog
    widget->m_text = _("Enter the new player's name");
    
    widget->m_properties[PROP_TEXT_ALIGN] = "center";
    widget->x = 0;
    widget->y = 0;
    widget->w = m_area.getWidth();
    widget->h = m_area.getHeight()/3;
    widget->setParent(m_irrlicht_window);
    
    m_children.push_back(widget);
    widget->add();
    
    // ----
    
    IGUIFont* font = GUIEngine::getFont();
    const int textHeight = font->getDimension(L"X").Height;
    
    const int textAreaYFrom = m_area.getHeight()/2 - textHeight/2;
    
    textCtrl = new TextBoxWidget();
    textCtrl->m_text = "";
    textCtrl->x = 50;
    textCtrl->y = textAreaYFrom - 10;
    textCtrl->w = m_area.getWidth()-100;
    textCtrl->h = textHeight + 5;
    textCtrl->setParent(m_irrlicht_window);
    m_children.push_back(textCtrl);
    textCtrl->add();
    textCtrl->setFocusForPlayer(0); // FIXME : don't hardcode player 0 ?
    
    // TODO : add Ok button

    cancelButton = new ButtonWidget();
    cancelButton->m_properties[PROP_ID] = "cancel";
    cancelButton->m_text = _("Cancel");
    cancelButton->x = 15;
    cancelButton->y = m_area.getHeight() - textHeight - 12;
    cancelButton->w = m_area.getWidth() - 30;
    cancelButton->h = textHeight + 6;
    cancelButton->setParent(m_irrlicht_window);
    
    m_children.push_back(cancelButton);
    cancelButton->add();

}
EnterPlayerNameDialog::~EnterPlayerNameDialog()
{
    textCtrl->getIrrlichtElement()->remove();
}
GUIEngine::EventPropagation EnterPlayerNameDialog::processEvent(std::string& eventSource)
{
    if(eventSource == "cancel")
    {
        dismiss();
        return GUIEngine::EVENT_BLOCK;
    }
    return GUIEngine::EVENT_LET;
}
void EnterPlayerNameDialog::onEnterPressedInternal()
{
    // ---- Cancel button pressed
    const int playerID = 0; // FIXME: don't ahrdcode player 0?
    if (GUIEngine::isFocusedForPlayer(cancelButton, playerID))
    {
        std::string fakeEvent = "cancel";
        processEvent(fakeEvent);
        return;
    }
        
    // ---- Otherwise, accept entered name
    stringw playerName = textCtrl->getText();
    if (playerName.size() > 0)
    {
        OptionsScreenPlayers::getInstance()->gotNewPlayerName( playerName );
    }
    
    // irrLicht is too stupid to remove focus from deleted widgets
    // so do it by hand
    GUIEngine::getGUIEnv()->removeFocus( textCtrl->getIrrlichtElement() );
    GUIEngine::getGUIEnv()->removeFocus( m_irrlicht_window );

    ModalDialog::dismiss();
}
