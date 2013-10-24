
class ColorRenderer
  include HtmlRenderer

  def initialize(color_map, index_only = false)
    @color_map = color_map
    super(index_only)
  end

  def do_render(element, value)
    if @color_map.has_key?(value) then
      %Q!<span style="color: #{@color_map[value]}">#{value}</span>!
    else
      value
    end
  end
end

colors = [
  "#EE4400",
  "#990099",
  "#0066FF",
  "#338833",
  "#006666",
  "#009900",
  "#000000",
]

colors2 = [
  "#EE0000",
  "#DD8800",
  "#338833",
  "#6666FF",
  "#000000",
]

status_colors = {}
@project.report_type['status'].each_with_index do |choice, i|
  colors << "#000000" if colors.size > i
  status_colors[choice.id] = colors[i]
end
ElementType.add_element_renderer(thread, 'status', ColorRenderer.new(status_colors, true))

priority_colors = {}
@project.report_type['priority'].each_with_index do |choice, i|
  colors2 << "#000000" if colors2.size > i
  priority_colors[choice.id] = colors2[i]
end
ElementType.add_element_renderer(thread, 'priority', ColorRenderer.new(priority_colors, true))

