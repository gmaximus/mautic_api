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
  
  $form['mautic_api_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic Client ID'),
    '#default_value' => variable_get('mautic_api_client_id', ''),
    '#size' => 70,
    '#description' => t("Your Mautic Client ID"),
    '#required' => TRUE,
  );
  
   $form['mautic_api_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Mautic Secret'),
    '#default_value' => variable_get('mautic_api_client_secret', ''),
    '#size' => 70,
    '#description' => t("Your Maultic Client Secret"),
    '#required' => TRUE,
  );
   
  // We add an additional submit handler to the stanard settings form
  $settings_form = system_settings_form($form);
  $settings_form['#submit'][] = 'maultic_api_admin_test_connection';
  return $settings_form;
}

/**
 * Additional submit handler for settings form. This will authorise the Drupal install with Mautic.
 * 
 * @param type $form
 * @param type $form_state
 */
function maultic_api_admin_test_connection($form, &$form_state) {
  $auth = mautic_api_get_auth();
  // Temporary dpm() call just to see the data we received upon authentication
  
  $access_token = variable_get('mautic_api_access_token');
  if ($access_token){
    drupal_set_message(t("We are connected to Mautic!"), 'status');
  }
  else {
    // Log the error
    drupal_set_message(t("Something did not work properly. Please check your logs!"), 'error');
  }
}
