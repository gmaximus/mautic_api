# Mautic API module 

**This is a work in progress.**

This module integrates Drupal 7 websites with a Mautic install. It leverages the [Mautic API Library](https://github.com/mautic/api-library) to achieve this.

The Drupal [Composer Manager](https://github.com/mautic/api-library) module is required to load their library into Drupal and install dependencies. That module of course requires the use of [Composer](https://getcomposer.org) itself.

It also adds a field to users to store their Mautic contact ID.

The whole idea for this module is to leverage rules. Give a new action a user and have that action provide a mautic_api_contact entity. We then do whatever we want with it. Then have a second action that we provide the mautic_api_contact and it will post it back to Mautic.

My use case is a Commerce site with affiliates. So my hope is to have fields in Mautic such as total spent or total earned. So if this module already existed that would become pretty trivial.

## Setting up Mautic for use with this module
1) In the settings menu visit "Configuration" and select the "Api settings" tab.
2) Ensure either the "API enabled?" (if using Oauth2) or "Enable HTTP basic auth?" (if using BasicAuth) is enabled. Please note that at this time only BasicAuth works. I'd be extremely grateful for any help with it. It works to an extent but won't stay authenticated.
3) If using BasicAuth, skip to 5. In the settings menu visit the "API credentials"
4) Click "+ new" to create a set of keys. You'll need these keys for Drupal. Use these settings:
  - For "Authorisation Protocol" select OAuth 2. 
  - For "Redirect URI" enter https://example.com/admin/config/services/mautic-api. Replace "example.com" in that with your Drupal website address.
5) Flush the Mautic cache. In the terminal go to the root of your Mautic install and run: rm -rf app/cache/*

## Install this Drupal module
1) Ensure the above dependencies are met.
2) Enable the module using Drush. 
3) In the terminal go to sites/default/files/composer and run: composer install
4) On your Drupal website visit admin/config/services/mautic-api and enter your Mautic information.
5) Click save.
6) If using Oauth2, you will be asked to authorise your website. 
7) Click Sync Mautic fields. This will replicate supported fields on the new Mautic API Contact entity type
That's is it at the moment. 

## What's Next?

Rules integration. 
- "Create / Fetch Mautic Contact" Action
  - We will provide a user object to our new action. 
  - We will search Mautic for the contact using the users email address. 
  - If not found we will create a new contact. 
  - We will then save all contact information to a Mautic API Contact entity.
- "Update Mautic Contact" Action. 
  - We will be able to use Rules "Set data value" to change whatever we want on the retrieved contact above. 
  - When we're done we can pass the updated contact to this action to update your Mautic install.
- "Mautic Contact Updated" Event. This will get triggered when a contact is updated in Mautic and posted to our endpoint ie On a Contact Preference Center form. We will make the mautic_api_contact available in Rules.

## A working example of using this module as it is

I have implemented a hook_form_FORM_ID_alter to add a new submit handler. I then post the information to Mautic using some functions provided by this module.

It's pretty light weight and you would have to tweak it to your fields and use case

```
/**
 * Implementation of hook_form_FORMID_alter
 * 
 * We extend the form to post settings to Mautic
 * 
 * @param type $form
 * @param type $form_state
 * @param type $form_id
 */
function MY_MODULE_form_FORMID_form_alter(&$form, &$form_state, $form_id) {
  // Add extra submit callback
    $form['actions']['submit']['#submit'][] = 'MY_MODULE_post_preferences_to_mautic';
}

/**
 * Our custom submit handler
 */
function MY_MODULE_post_preferences_to_mautic($form, &$form_state) {
   
  // Set the contact preferences being submitted.
    $entity = $form['field_cp_designer_news_pref']['und']['#entity']; // This only works with my form 
    
   // We build this array with our new values.
    $data = array();
  
  // Get user info
    $uid = $entity->uid;
    $user = user_load($uid);
    $email = $user->mail;
  
  // Get Mautic Contact ID
    $mautic_id = $user->mautic_api_contact_id;
  // Check if we already have a Contact ID  
    if (empty($mautic_id)) {
     // Check if a contact already exists in Mautic 
      $mautic_contact = mautic_api_get_contact($email);
      if ($mautic_contact) {
        $mautic_contact_id = $mautic_contact['id'];
      }
     // Otherwise create a new contact
      else {
        $new_contact_data = array(
         'email' => $email,
         'ipAddress' => ip_address(), // Drupal core function.
        );
        $new_contact = mautic_api_create_contact($new_contact_data);
        $mautic_contact_id = $new_contact['contact']['id'];
       
      }
      // Save the Mautic Contact ID on the user
        $user_update = array();
        $user_update['mautic_api_contact_id'][LANGUAGE_NONE][0]['value'] = $mautic_contact_id;
        user_save($user, $user_update);
    }
    else {
       $mautic_contact_id = $user->mautic_api_contact_id['und'][0]['value'];
    }
    

  // Set some values. You would need to adapt of course.

  // Set Affiliate news
    $drupal_affiliate_news = $entity->field_cp_affiliate_news_pref['und'][0]['value'];
    $data['affiliate_news'] = $drupal_affiliate_news;
  
  // Set Buyer news
    $drupal_buyer_news = $entity->field_cp_buyer_news_pref['und'][0]['value'];
    $data['buyer_news'] = $drupal_buyer_news;
    
  // Set Email updates
    $drupal_email_updates = $entity->field_cp_email_updates['und'][0]['value'];
    $data['email_updates'] = $drupal_email_updates;
    
  // Set SMS updates
    $drupal_sms_updates = $entity->field_cp_sms_updates['und'][0]['value'];
    $data['sms_updates'] = $drupal_sms_updates;
  
  // Post settings to Mautic
   mautic_api_edit_contact($mautic_contact_id, $data);
     
}
```