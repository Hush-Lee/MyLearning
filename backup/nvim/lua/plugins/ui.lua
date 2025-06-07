-- Neovim 美化 + IDE 补全配置集合
-- 路径建议：lua/plugins/ui.lua

return {
  -- 🌈 主题
  {
    "catppuccin/nvim",
    name = "catppuccin",
    priority = 1000,
    config = function()
      vim.cmd.colorscheme("catppuccin-mocha")
    end,
  },

  -- ⬇️ 图标支持
  { "nvim-tree/nvim-web-devicons", lazy = true },

  -- 📂 文件树
  {
    "nvim-tree/nvim-tree.lua",
    config = function()
      require("nvim-tree").setup({
        view = { width = 30, side = "left" },
        renderer = { icons = { show = { file = true, folder = true, folder_arrow = true } } },
      })
    end,
  },

  -- 📊 状态栏
  {
    "nvim-lualine/lualine.nvim",
    dependencies = { "nvim-tree/nvim-web-devicons" },
    config = function()
      require("lualine").setup({
        options = {
          theme = "catppuccin",
          section_separators = { left = "", right = "" },
          component_separators = "|",
        },
      })
    end,
  },

  -- 📌 标签栏
  {
    "akinsho/bufferline.nvim",
    version = "*",
    dependencies = "nvim-tree/nvim-web-devicons",
    config = function()
      require("bufferline").setup({
        options = { mode = "buffers", diagnostics = "nvim_lsp", separator_style = "slant" },
      })
    end,
  },

  -- 🏠 启动页
  {
    "goolord/alpha-nvim",
    config = function()
      require("alpha").setup(require("alpha.themes.dashboard").config)
    end,
  },

  -- 📜 消息增强
  {
    "folke/noice.nvim",
    dependencies = { "MunifTanjim/nui.nvim" },
    config = function()
      require("noice").setup()
    end,
  },

  -- 🌬 平滑滚动
  {
    "echasnovski/mini.animate",
    version = "*",
    config = function()
      require("mini.animate").setup()
    end,
  },

  -- ✨ 自动括号 + 补全联动
  {
    "windwp/nvim-autopairs",
    event = "InsertEnter",
    config = function()
      local autopairs = require("nvim-autopairs")
      autopairs.setup()
      local cmp_autopairs = require("nvim-autopairs.completion.cmp")
      local cmp_status_ok, cmp = pcall(require, "cmp")
      if cmp_status_ok then
        cmp.event:on("confirm_done", cmp_autopairs.on_confirm_done())
      end
    end,
  },

  -- 🔧 补全 + LSP
  { "hrsh7th/nvim-cmp" },
  { "hrsh7th/cmp-nvim-lsp" },
  { "hrsh7th/cmp-buffer" },
  { "hrsh7th/cmp-path" },
  { "hrsh7th/cmp-cmdline" },
  { "saadparwaiz1/cmp_luasnip" },
  { "L3MON4D3/LuaSnip" },
  { "rafamadriz/friendly-snippets" },

  -- 📦 LSP 支持（不使用 mason）
  { "neovim/nvim-lspconfig" },

  -- 🛠 LSP & 补全配置（不使用 mason）
  {
    "VonHeikemen/lsp-zero.nvim",
    branch = "v3.x",
    dependencies = {
      "neovim/nvim-lspconfig",
      "hrsh7th/nvim-cmp",
      "hrsh7th/cmp-nvim-lsp",
      "hrsh7th/cmp-buffer",
      "hrsh7th/cmp-path",
      "hrsh7th/cmp-cmdline",
      "saadparwaiz1/cmp_luasnip",
      "L3MON4D3/LuaSnip",
      "rafamadriz/friendly-snippets",
    },
    config = function()
      require("luasnip.loaders.from_vscode").lazy_load()

      local cmp = require("cmp")
      local luasnip = require("luasnip")
      cmp.setup({
        snippet = {
          expand = function(args)
            luasnip.lsp_expand(args.body)
          end,
        },
        mapping = cmp.mapping.preset.insert({
          ["<Tab>"] = cmp.mapping(function(fallback)
            if cmp.visible() then
              cmp.select_next_item()
            elseif luasnip.expand_or_jumpable() then
              luasnip.expand_or_jump()
            else
              fallback()
            end
          end, { "i", "s" }),
          ["<CR>"] = cmp.mapping.confirm({ select = true }),
        }),
        sources = cmp.config.sources({
          { name = "nvim_lsp" },
          { name = "luasnip" },
          { name = "buffer" },
          { name = "path" },
        })
      })
    end,
  },
}
