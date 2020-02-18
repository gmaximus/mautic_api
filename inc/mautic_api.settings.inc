<?php

/**
 * This defines the admin settings form for mautic_api.
 * 
 * @return string
 */

function mautic_api_settings_form() {
  
  $form = array();

  $form['mautic_api_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic URL'),
    '#default_value' => variable_get('mautic_api_base_url', ''),
    '#size' => 70,
    '#description' => t("Your mautic base url, e.g. https://mydomain.com. No trailing slash required."),
    '#required' => TRUE,
  );
  
  $form['mautic_api_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic user ID'),
    '#default_value' => variable_get('mautic_api_user_id', ''),
    '#size' => 70,
    '#description' => t("The Mautic user ID that new contacts should be assigned to."),
    '#required' => TRUE,
  );
  
  $form['mautic_api_signup_block_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Signup Block Info.'),
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );
  
  $form['mautic_api_signup_block_fieldset']['mautic_api_signup_form_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic Form ID'),
    '#default_value' => variable_get('mautic_api_signup_form_id', ''),
    '#size' => 70,
    '#description' => t("Enter the Mautic Form ID for the signup block. The form should have one field for a email. This should have the machine name (Field HTML name in Mautic): email_address"),
    '#required' => FALSE,
  );
  
  $above_form_value = variable_get('mautic_api_above_form', array('value' => '', 'format' => NULL));
  $form['mautic_api_signup_block_fieldset']['mautic_api_above_form'] = array(
    '#type' => 'text_format',
    '#title' => t('Above form'),
    '#default_value' => $above_form_value['value'],
    '#format' => $above_form_value['format'],
    '#size' => 70,
    '#description' => t("This will be displayed above the form in the block."),
    '#required' => FALSE,
  );
  
  $below_form_value = variable_get('mautic_api_below_form', array('value' => '', 'format' => NULL));
  $form['mautic_api_signup_block_fieldset']['mautic_api_below_form'] = array(
    '#type' => 'text_format',
    '#title' => t('Below form'),
    '#default_value' => $below_form_value['value'],
    '#format' => $below_form_value['format'],
    '#size' => 70,
    '#description' => t("This will be displayed blow the form in the block."),
    '#required' => FALSE,
  );
  
  $form['mautic_api_signup_block_fieldset']['mautic_api_signup_success'] = array(
    '#type' => 'textarea',
    '#title' => t('Success message'),
    '#default_value' => variable_get('mautic_api_signup_success', ''),
    '#size' => 70,
    '#description' => t("This will be displayed upon a successful submission."),
    '#required' => FALSE,
  );
  
  $form['mautic_api_auth_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'basic_auth'   => t('BasicAuth'),
      'oauth2' => t('Oauth2'),
    ),
    '#default_value' => variable_get('mautic_api_auth_type', ''),
    '#title' => t('Authentication type'),
    '#description' => t("Only BasicAuth works. Oauth2 is buggy and I gave up. Please share if you fix it."),
  );
   
  $form['mautic_api_oauth2_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Oauth2 Information.'),
    '#states' => array(
      'visible' => array(
        ':input[name="mautic_api_auth_type"]' => array('value' => 'oauth2'),
      ),
    ),
  );
   
  $form['mautic_api_oauth2_fieldset']['mautic_api_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic Client ID'),
    '#default_value' => variable_get('mautic_api_client_id', ''),
    '#size' => 70,
    '#description' => t("Your Mautic Client ID"),
    '#required' => TRUE,
  );
  
  $form['mautic_api_oauth2_fieldset']['mautic_api_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic Client Secret'),
    '#default_value' => variable_get('mautic_api_client_secret', ''),
    '#size' => 70,
    '#description' => t("Your Maultic Client Secret"),
    '#required' => TRUE,
  );
  
  
  $form['mautic_api_oauth2_fieldset']['test_connention'] = array(
    '#type' => 'submit',
    '#value' => t('Authorise App'),
    '#submit' => array('maultic_api_admin_test_connection'),
    '#weight' => 200,
  );
  
  $form['mautic_api_basic_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('BasicAuth Information.'),
    '#states' => array(
      'visible' => array(
        ':input[name="mautic_api_auth_type"]' => array('value' => 'basic_auth'),
      ),
    ),
  );
  
  $form['mautic_api_basic_fieldset']['mautic_api_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic user name'),
    '#default_value' => variable_get('mautic_api_user_name', ''),
    '#size' => 70,
    '#description' => t("Your Maultic user name. "),
    '#required' => TRUE,
  );
  
  $form['mautic_api_basic_fieldset']['mautic_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic password'),
    '#default_value' => variable_get('mautic_api_password', ''),
    '#size' => 70,
    '#description' => t("Your Mautic password"),
    '#required' => TRUE,
  );
  
  $form['actions']['sync_fields'] = array(
    '#type' => 'submit',
    '#value' => t('Sync Mautic fields'),
    '#submit' => array('mautic_api_sync_contact_fields'),
    '#weight' => 200,
  );
     
  // We add an additional submit handler to the stanard settings form
  $settings_form = system_settings_form($form);
  // $settings_form['#submit'][] = 'maultic_api_admin_test_connection';
 
  return $settings_form;
}

/**
 * Additional submit handler for settings form. This will authorise the Drupal install with Mautic.
 * 
 * @param type $form
 * @param type $form_state
 */
function maultic_api_admin_test_connection($form, &$form_state) {
  
  // Sign in
  $auth = mautic_api_get_auth();  
  $access_token = variable_get('mautic_api_access_token');
  if ($access_token){
    drupal_set_message(t("We are connected to Mautic!"), 'status');
  }
  else {
    // Log the error
    drupal_set_message(t("Something did not work properly. Please check your logs!"), 'error');
  }
}

/**
 * The callback for Sync Mautic fields button
 *  
 * @param type $form
 * @param type $form_state
 */
function mautic_api_sync_contact_fields($form, &$form_state) {
  
  // Get the data from Mautic about the contact fields
    $mautic_fields = mautic_api_get_contact_field_list();
  
  // Initilise the batch api operation to sync fields.
    $batch = array(
      'operations' => array(
        array('mautic_api_sync_fields_batch_run', array($mautic_fields)),
        ),
      'finished' => 'mautic_api_sync_fields_batch_finished',
      'title' => t('Processing Mautic fields'),
      'init_message' => t('Sync fields is starting.'),
      'progress_message' => t('Processed @current out of @total fields.'),
      'error_message' => t('Mautic API Sync Fields has encountered an error.'),
      'file' => drupal_get_path('module', 'mautic_api') . '/inc/mautic_api.sync_fields.batch.inc',
    );
    batch_set($batch);
}
