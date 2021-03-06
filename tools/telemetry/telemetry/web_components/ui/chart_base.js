// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict';

tvcm.require('tvcm.utils');
tvcm.require('telemetry.web_components.ui.d3');
tvcm.requireTemplate('telemetry.web_components.ui.chart_base');

tvcm.exportTo('telemetry.web_components.ui', function() {
  var svgNS = 'http://www.w3.org/2000/svg';

  function getColorOfKey(key) {
    var id = tracing.getStringColorId(key);
    return tracing.getColorPalette()[id];
  }

  /**
   * A virtual base class for basic charts that provides X and Y axes, if
   * needed, a title, and legend.
   *
   * @constructor
   */
  var ChartBase = tvcm.ui.define('svg', undefined, svgNS);

  ChartBase.prototype = {
    __proto__: HTMLUnknownElement.prototype,

    decorate: function() {
      this.classList.add('chart-base');
      this.chartTitle_ = undefined;
      this.data_ = undefined;
      this.seriesKeys_ = undefined;
      this.width_ = 400;
      this.height_ = 300;

      // This should use tvcm.instantiateTemplate. However, creating
      // svg-namespaced elements inside a template isn't possible. Thus, this
      // hack.
      var template = document.head.querySelector('#chart-base-template');
      var svgEl = template.content.querySelector('svg');
      for (var i = 0; i < svgEl.children.length; i++)
        this.appendChild(svgEl.children[i].cloneNode(true));
    },

    get chartTitle() {
      return chartTitle_;
    },

    set chartTitle(chartTitle) {
      this.chartTitle_ = chartTitle;
      this.updateContents_();
    },

    get chartAreaElement() {
      return this.querySelector('#chart-area');
    },

    get width() {
      return width_;
    },

    set width(width) {
      this.width_ = width;
      this.updateContents_();
    },

    get height() {
      return height_;
    },

    set height(height) {
      this.height_ = height;
      this.updateContents_();
    },

    get data() {
      return this.data_;
    },

    get margin() {
      var margin = {top: 20, right: 20, bottom: 30, left: 50};
      if (this.chartTitle_)
        margin.top += 20;
      return margin;
    },

    get chartAreaSize() {
      var margin = this.margin;
      return {
        width: this.width_ - margin.left - margin.right,
        height: this.height_ - margin.top - margin.bottom
      };
    },

    getLegendKeys_: function() {
      throw new Error('Not implemented');
    },

    updateScales_: function(width, height) {
      throw new Error('Not implemented');
    },

    updateContents_: function() {
      var margin = this.margin;
      var width = this.chartAreaSize.width;
      var height = this.chartAreaSize.height;

      var thisSel = d3.select(this);
      thisSel.attr('width', this.width_);
      thisSel.attr('height', this.height_);

      var chartAreaSel = d3.select(this.chartAreaElement);
      chartAreaSel.attr(
          'transform',
          'translate(' + margin.left + ',' + margin.top + ')');

      this.updateScales_(width, height);

      // Axes.
      if (this.xScale_ && this.yScale_) {
        var xAxisRenderer = d3.svg.axis()
            .scale(this.xScale_)
            .orient('bottom');

        var yAxisRenderer = d3.svg.axis()
            .scale(this.yScale_)
            .orient('left');

        chartAreaSel.select('.x.axis')
            .attr('transform', 'translate(0,' + height + ')')
            .call(xAxisRenderer);

        chartAreaSel.select('.y.axis')
            .call(yAxisRenderer);
      }

      // Title.
      var titleSel = chartAreaSel.select('#title');
      if (this.chartTitle_) {
        titleSel.attr('transform', 'translate(' + width * 0.5 + ',-5)')
            .style('display', undefined)
            .style('text-anchor', 'middle')
            .attr('class', 'title')
            .attr('width', width)
            .text(this.chartTitle_);
      } else {
        titleSel.style('display', 'none');
      }

      this.updateLegend_();
    },

    updateLegend_: function() {
      var keys = this.getLegendKeys_();

      var chartAreaSel = d3.select(this.chartAreaElement);
      var chartAreaSize = this.chartAreaSize;

      var legendEntriesSel = chartAreaSel.selectAll('.legend')
          .data(keys.slice().reverse());

      legendEntriesSel.enter()
          .append('g')
          .attr('class', 'legend')
          .attr('transform', function(d, i) {
            return 'translate(0,' + i * 20 + ')';
          }).append('text').text(function(key) {
            return key;
          });
      legendEntriesSel.exit().remove();

      legendEntriesSel.attr('x', chartAreaSize.width - 18)
          .attr('width', 18)
          .attr('height', 18)
          .style('fill', function(key) {
            return getColorOfKey(key);
          });

      legendEntriesSel.selectAll('text')
        .attr('x', chartAreaSize.width - 24)
        .attr('y', 9)
        .attr('dy', '.35em')
        .style('text-anchor', 'end')
        .text(function(d) { return d; });
    }
  };

  return {
    getColorOfKey: getColorOfKey,
    ChartBase: ChartBase
  };
});
