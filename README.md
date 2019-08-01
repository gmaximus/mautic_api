This is a Drupal 7 module to provide integration with your Mautic installation. 

The Mautic API module merely registers the API library. It does nothing other than that inc loading it. 

This requires installing [xAutoload module](https://www.drupal.org/project/xautoload) first.

To install
1) Download and extract the contents of the [Mautic API Library](https://github.com/mautic/api-library/archive/master.zip) into your libraries folder. Put it in a folder called mautic. 
ie sites/all/libraries/mautic/lib/MauticApi.php
2) Turn on module as normal.

The Mautic API module uses the above library to connect to your Mautic installation using OAuth2. 

To install:
1) Ensure the above steps are complete.
2) Turn on module and visit admin/config/system/mautic-api and enter your API settings.
3) Click save to authorise your website using the info provided.

That's is it at the moment. 
........

Planned features:

Rules integration. 
- Action to create and/or fetch a contact from Mautic. 
  - We will provide a user object to our new action. 
  - We will search Mautic for the contact using the users email address. 
  - If not found we will create a new contact. 
  - We will then make all fields available in your Rule.
- Action to update the contact in Mautic. 
  - We will be able to use Rules "Set data value" to change whatever we want on the retrieved contact. 
  - When we're done we can pass the updated contact to this action for the update.
