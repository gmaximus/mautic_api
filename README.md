# Mautic API module 

**This is a work in progress and not ready to use as is. Only published for collaboration purposes.**

This module integrates Drupal 7 websites with a Mautic install. It leverages the [Mautic API Library](https://github.com/mautic/api-library) to achieve this.

The Drupal [Composer Manager](https://github.com/mautic/api-library) module is required to load their library into Drupal. That module of course requires the use of [Composer](https://getcomposer.org) itself.

It also adds a field to users to store their Mautic contact ID.

## Setting up Mautic for use with this module
1) In the settings menu visit "Configration" and select the "Api settings" tab.
2) Ensure the API is enabled.
3) In the settings menu visit the "API credentials"
4) Click "+ new" to create a set of keys. You'll need these keys for Drupal. Use these settings:
  - For "Authorisation Protocol" select OAuth 2. 
  - For "Redirect URI" enter https://your-website.com/admin/config/system/mautic-api. Replace "your-website.com" in that with your Drupal website address.
5) Flush the Mautic cache. In the terminal go to the root of your Mautic install and run: rm -rf app/cache/*

## Install this Drupal module
1) Ensure the above dependencies are met.
2) Enable the module using Drush. 
3) In the terminal go to sites/default/files/composer & run: composer install
4) On your Drupal website visit admin/config/system/mautic-api and enter your Mautic information.
5) Click save to authorise your website. When you get returned to Drupal, click save again and you should see a success message. If not, check your logs for more info.

That's is it at the moment. 

## What's Next?

Rules integration. 
- Action to create and/or fetch a contact from Mautic. 
  - We will provide a user object to our new action. 
  - We will search Mautic for the contact using the users email address. 
  - If not found we will create a new contact. 
  - We will then make all fields available in your Rule.
- Action. Update the contact in Mautic. 
  - We will be able to use Rules "Set data value" to change whatever we want on the retrieved contact above. 
  - When we're done we can pass the updated contact to this action to update your Mautic install.
- Event. This will get triggered when a contact is updated in Mautic and posted to our endpoint ie On a Contact Preference Center form. We will make the mautic_contact available in Rules.