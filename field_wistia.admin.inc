<?php

/**
 * @file
 * Settings form for Wistia field module.
 *
 * Sets the default settings and variables for the field globally
 * Can be overriden on and individual field basis
 */

/**
 * Implements hook_settings_form().
 */
function field_wistia_settings_form($form) {
  $form['text'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('The following settings will be used as default values
      on all Wistia video fields.  Many of these settings can be overridden
      on a per-field basis.') . '</p>',
  );
  $form['field_wistia_embed'] = array(
    '#title' => t('Set Default Embed Type'),
    '#type' => 'radios',
    '#options' => array(
      'iframe' => t('iFrame'),
      'api' => t('API'),
      'seo' => t('SEO'),
    ),
    '#default_value' => variable_get('field_wistia_embed', FALSE),
  );
  $form['field_wistia_videofoam'] = array(
    '#type' => 'checkbox',
    '#title' => t('Scale video window to device with VideoFoam'),
    '#default_value' => variable_get('field_wistia_videofoam', TRUE),
    '#description' => l(t('VideFoam Info'), 'http://wistia.com/labs/videofoam'),
  );
  $form['field_wistia_autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wistia autoplay'),
    '#default_value' => variable_get('field_wistia_autoplay', TRUE),
  );
  $form['field_wistia_thumb_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Wistia Field thumbnail directory'),
    '#field_prefix' => variable_get('file_public_path', conf_path() . '/files') . '/',
    '#field_suffix' => '/thumbnail.png',
    '#description' => t('Location, within the files directory, where you would like the Wistia thumbnails stored.'),
    '#default_value' => variable_get('field_wistia_thumb_dir', 'field_wistia'),
  );

  return system_settings_form($form);
}
