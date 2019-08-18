<?php

function mautic_api_sync_fields_batch_run($mautic_fields, &$context) {
  
  // Count the total volume of fields that need to be processed.
    $total_mautic_field_count = count($mautic_fields);
  
  // Check if this is the first run. If so configure the settings for Batch API.
    if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['max'] = $total_mautic_field_count;
    }
   
  // Limit the fields processed on each batch
    $progress = $context['sandbox']['progress'];
    $limit = 5;
    $total_to_process = $progress + $limit;
  // Make array of field ID's. This way we can work through the next X fields.
    $field_ids = array_column($mautic_fields, 'id');
    
  // Loop over Mautic field info
    while ($progress < $total_to_process && $progress < $total_mautic_field_count) {

      // Create the field in Drupal
        $field_id = $field_ids[$progress];
        $mautic_field = $mautic_fields[$field_id];
        mautic_api_sync_contact_field($mautic_field);

      // Update our progress information.
        $progress++;
        $context['sandbox']['progress']++;
        $context['results'][] = check_plain($mautic_field['label']);
        $context['message'] = t('Now processing Batch API overview', array('Batch API overview' => $mautic_field['label']));
    }

    // Inform the batch engine that we are not finished,
    // and provide an estimation of the completion level we reached.
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 * Batch 'finished' callback
 */
function mautic_api_sync_fields_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = t('@count Mautic fields successfully synced', array('@count' => count($results)));
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
  
}

/**
 * This takes a Mautic field definition
 * 
 * 1) Checks to see if the Drupal field exists.
 * 2) Use the info to create a Drupal field definition.
 * 3) Finally add the field to the mautic_api_contact entity type.
 * 
 * @param type $mautic_field
 */
function mautic_api_sync_contact_field($mautic_field) {
  
  // Set the local variables for the data we have to work with.
  $mautic_field_alias = $mautic_field['alias'];  
  $mautic_field_type = $mautic_field['type'];
  $mautic_field_label = $mautic_field['label'];
  $mautic_field_id = $mautic_field['id'];
  $drupal_field_name = 'mac_' . $mautic_field_alias;
  
  // Check if this field is present. If not, we will create it.
  $instance = field_info_instance('mautic_api_contact', $drupal_field_name, 'mautic_api_contact');
  if (!$instance) {
        
    switch ($mautic_field_type) {

      case 'text':
          $field = array(
            'field_name' => $drupal_field_name,
            'type' => 'text',
          );
          

          $instance = array(
            'field_name' => $drupal_field_name,
            'entity_type' => 'mautic_api_contact',
            'bundle' => 'mautic_api_contact',
            'label' => $mautic_field_label,
          );
          
        break;

      case 'boolean':
        $field_info = mautic_api_get_contact_field_info($mautic_field_id);
        $field_options = $field_info['field']['properties'];
        $field = array(
          'field_name' => $drupal_field_name,
          'type' => 'list_boolean',
          'settings' => array(
             'allowed_values' => $field_options,
           ),
         );
        $instance = array(
          'field_name' => $drupal_field_name,
          'entity_type' => 'mautic_api_contact',
          'bundle' => 'mautic_api_contact',
          'label' => $mautic_field_label,
         );
        break;

      case 'email':
          $field = array(
            'field_name' => $drupal_field_name,
            'type' => 'email',
          );
          $instance = array(
            'field_name' => $drupal_field_name,
            'entity_type' => 'mautic_api_contact',
            'bundle' => 'mautic_api_contact',
            'label' => $mautic_field_label,
          );
        break;

      case 'phone':
          $field = array(
            'field_name' => $drupal_field_name,
            'type' => 'text',
          );
          $instance = array(
            'field_name' => $drupal_field_name,
            'entity_type' => 'mautic_api_contact',
            'bundle' => 'mautic_api_contact',
            'label' => $mautic_field_label,
          );
        break;
      case 'number':
          $field_info = mautic_api_get_contact_field_info($mautic_field_id);
          $field_options = $field_info['field']['properties'];
          $field = array(
            'field_name' => $drupal_field_name,
            'type' => 'number_decimal',
            'settings' => array(
              'precision' => 15, // Couldn't get this info from Mautic. Figured this would cover most scenarios
              'scale' => $field_options['precision'], // Mautic call "scale" - "precision"
              'decimal_separator' => '.',
            ),
          );
          $instance = array(
            'field_name' => $drupal_field_name,
            'entity_type' => 'mautic_api_contact',
            'bundle' => 'mautic_api_contact',
            'label' => $mautic_field_label,
          );
        break;

      case 'country':
         $field = array(
            'field_name' => $drupal_field_name,
            'type' => 'text',
          );
          $instance = array(
            'field_name' => $drupal_field_name,
            'entity_type' => 'mautic_api_contact',
            'bundle' => 'mautic_api_contact',
            'label' => $mautic_field_label,
          );
        break;

      default:
       $field = array(
          'type' => 'not-implemented',
        );
        break;
    } 
    
    if ($field['type'] != 'not-implemented') {
      field_create_field($field);
      field_create_instance($instance);
    }
  }
}

function mautic_api_delete_contact_field($mautic_field) {
  // Set the local variables for the data we have to work with.
  $mautic_field_alias = $mautic_field['alias'];
  $drupal_field_name = 'mac_' . $mautic_field_alias;
  
  // Check if this field is present. If not, we will create it.
  $instance = field_info_instance('mautic_api_contact', $drupal_field_name, 'mautic_api_contact');
  if ($instance) {
    field_delete_instance($instance);
  }
}