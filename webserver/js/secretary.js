var ControlPanel = null;
define ( [ "ControlPanelInteraction"
		 , 'domReady'
		 ]
	   , function(ControlPanelInteraction, domReady) {
			 ControlPanel = ControlPanelInteraction;
			 domReady( function() {
				 console.log(ControlPanel);
				 ControlPanel.init();
				});
			 return ControlPanel;
			}
);
