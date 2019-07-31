This is a Drupal 7 module to provide integration with Mautic. 

The mautic_api_library module merely registers the API library. It does nothing other than that inc loading it. To install

1) Download and extract the contents of https://github.com/mautic/api-library/archive/master.zip into your libraries folder in a folder called mautic. 
ie sites/all/libraries/mautic/lib/MauticApi.php
2) Turn on module as normal.

The mautic_api module uses the above library to connect to your Mautic installation using OAuth2. To install:
1) Ensure the above steps are complete.
2) Turn on module and visit admin/config/system/mautic-api and enter your API settings.
3) Click save to authorise your website using the info provided.
........

That's is it at the moment. Planned features:

- Rules integration. Actions to create and update contacts.