#!/usr/bin/env python

import os
import sys
sys.path.append(os.path.join(os.path.dirname(__file__), '../..'))

import os.path
import subprocess
from name_utilities import enum_for_css_keyword
import json5_generator

import template_expander
import gperf


class CSSValueKeywordsWriter(json5_generator.Writer):
    def __init__(self, file_paths):
        json5_generator.Writer.__init__(self, file_paths)
        self._outputs = {
            "CSSValueKeywords.h": self.generate_header,
            "CSSValueKeywords.cpp": self.generate_implementation
        }

        self._value_keywords = self.json5_file.name_dictionaries
        first_keyword_id = 1
        for offset, keyword in enumerate(self._value_keywords):
            keyword['lower_name'] = keyword['name'].lower()
            keyword['enum_name'] = enum_for_css_keyword(keyword['name'])
            keyword['enum_value'] = first_keyword_id + offset
            if keyword['name'].startswith('-internal-'):
                assert keyword['mode'] is None, 'Can\'t specify mode for ' \
                    'value keywords with the prefix "-internal-".'
                keyword['mode'] = 'UASheet'
            else:
                assert keyword['mode'] != 'UASheet', 'UASheet mode only ' \
                    'value keywords should have the prefix "-internal-".'
        self._keyword_count = len(self._value_keywords) + first_keyword_id

    @template_expander.use_jinja('core/css/templates/CSSValueKeywords.h.tmpl')
    def generate_header(self):
        return {
            'value_keywords': self._value_keywords,
            'value_keywords_count': self._keyword_count,
            'max_value_keyword_length':
                max(len(keyword['name']) for keyword in self._value_keywords),
        }

    def _value_keywords_with_mode(self, mode):
        return [
            keyword for keyword in self._value_keywords
            if keyword['mode'] == mode
        ]

    @gperf.use_jinja_gperf_template(
        'core/css/templates/CSSValueKeywords.cpp.tmpl',
        ['-Q', 'CSSValueStringPool'])
    def generate_implementation(self):
        keyword_offsets = []
        current_offset = 0
        for keyword in self._value_keywords:
            keyword_offsets.append(current_offset)
            current_offset += len(keyword["name"]) + 1

        return {
            'value_keywords': self._value_keywords,
            'value_keyword_offsets': keyword_offsets,
            'ua_sheet_mode_values_keywords':
                self._value_keywords_with_mode('UASheet'),
            'quirks_mode_or_ua_sheet_mode_values_keywords':
                self._value_keywords_with_mode('QuirksOrUASheet'),
            'gperf_path': self.gperf_path,
        }

if __name__ == "__main__":
    json5_generator.Maker(CSSValueKeywordsWriter).main()
